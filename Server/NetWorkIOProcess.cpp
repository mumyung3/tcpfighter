#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "NetWorkIOProcess.h"
#include <WS2tcpip.h>
#include "GameUpdate.h"
#include <stdio.h>

// 임시 버퍼 삭제후, 로컬 버퍼 없는 링버퍼 적용 예정
#define TEMP_BUFSIZE 10000

void NetIOProcess() {
	st_SESSION* pSession;

	FD_SET ReadSet;
	FD_SET WriteSet;

	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);

	// 리슨 소켓 넣기
	FD_SET(g_ListenSocket, &ReadSet);

	//리슨 소켓 및 접속중인 모든 클라이언트에 대해 socket 체크
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pSession = &(*it);

		// 해당 클라이언트 read set 등록 sendq에 데이터 있다면 writeset 등록
		FD_SET(pSession->Socket, &ReadSet);
		if (pSession->SendQ.GetUseSize() > 0)
			FD_SET(pSession->Socket, &WriteSet);
	}

	// 게임 프레임 처리를 위한 timeval 0,0
	timeval Time{};

	int iResult{}, iCnt{};
	bool bProcFlag{};

	Time.tv_sec = 0;
	Time.tv_usec = 0;
	iResult = select(0, &ReadSet, &WriteSet, 0, &Time);

	if (iResult > 0) {
		if (FD_ISSET(g_ListenSocket, &ReadSet)) {
			netProc_Accept();
		}

		// 전체 세션 반응 확인
		for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
			st_SESSION* pSession = &(*it);
			if (FD_ISSET(pSession->Socket, &ReadSet)) {
				--iResult;
				netProc_Recv(pSession);
			}

			if (FD_ISSET(pSession->Socket, &WriteSet)) {
				--iResult;
				netProc_Send(pSession);
			}


		}
	}
	else if (iResult == SOCKET_ERROR) {

		int errCode = WSAGetLastError();
		g_bShutdown = true;

		wprintf(L"select 실패 : %d\n", errCode);
		__debugbreak();
		return;
	}

	//지연 삭제
	CleanupDisconnected();


}

void netProc_Accept() {
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSock = accept(g_ListenSocket, (SOCKADDR*)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET) {
		wprintf(L"accept 실패 : %d\n", WSAGetLastError());
		return;
	}

	// g_playerlist 추가
	st_SESSION* newSession = new st_SESSION();
	newSession->Socket = clientSock;
	newSession->dwSessionID = g_ID;

	// 플레이어 생성 패킷 생성
	PacketCreatePlayer Packet{};
	PacketHeader Header = CreatePacketHeader();

	CreatePacketPlayer(&Header, &Packet);

	newSession->chHP = Packet.HP;
	newSession->shX = Packet.X;
	newSession->shY = Packet.Y;
	newSession->dwAction = dfACTION_STOP;

	InetNtopW(AF_INET, &clientAddr.sin_addr, newSession->ip, 16);
	newSession->port = ntohs(clientAddr.sin_port);

	SendUnicast(newSession, &Header, (void*)&Packet);

	newSession->bDisconnect = false;
	newSession->next = g_PlayerList.head;
	g_PlayerList.head = newSession;


	wprintf(L"클라 접속 : IP=%s PORT=%d ID=%d\n", newSession->ip, newSession->port, newSession->dwSessionID);


	// 새 플레이어 -> 기존 클라 전송
	PacketHeader NewHeader = CreatePacketHeader();
	PacketCreateOtherPlayer NewPacket{};
	CreatePacketOtherPlayer(&NewHeader, &NewPacket, Packet.ID, Packet.Direction, Packet.X, Packet.Y, Packet.HP);
	SendBroadcast(newSession, &NewHeader, &NewPacket);

	// 기존 플레이어 -> 새 클라 전송
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pOther = &(*it);
		if (pOther == newSession) continue;
		PacketHeader OtherHeader = CreatePacketHeader();
		PacketCreateOtherPlayer OtherPacket{};

		CreatePacketOtherPlayer(&OtherHeader, &OtherPacket, pOther->dwSessionID, pOther->byDirection, pOther->shX, pOther->shY, pOther->chHP);
		SendUnicast(newSession, &OtherHeader, &OtherPacket);
	}
	g_ID++;

	// netProc_Accept
	wprintf(L"Create Character # SessionID:%d\tX:%d\tY:%d\n", newSession->dwSessionID, newSession->shX, newSession->shY);


}

void netProc_Recv(st_SESSION* pSession) {

	char chTemp[TEMP_BUFSIZE] = {};

	// recv 전에 체크 추가
	if (pSession->RecvQ.DirectEnqueueSize() == 0) return;

	int recvRet = recv(pSession->Socket, pSession->RecvQ.GetRearBufferPtr(), pSession->RecvQ.DirectEnqueueSize(), 0);


	//recv 리턴값으로 종료 / 에러 socket 에러 처리
	if (recvRet == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			// 접속 불량
			// 연결 끊기
			Disconnect(pSession);
			return;
			// 다른 사용자는 계속 돌릴 예정
		}
	}
	if (recvRet == 0) {
		// 정상 종료
		Disconnect(pSession);
		return;
	}


	// 데이터 받았다면 일단 recvq 인큐
	// 링버퍼 인큐 및 성공 여부 처리
	if (pSession->RecvQ.MoveRear(recvRet) != recvRet) {
		Disconnect(pSession);
		return;
	}


	// 완료 패킷 처리
	while (1) {

		// 1. recvq에 최소한의 사이즈 확인 - 헤더 사이즈 초과
		if (pSession->RecvQ.GetUseSize() < sizeof(PacketHeader))	break;
		// 2. recvq에서 헤더 peek
		PacketHeader header{};
		pSession->RecvQ.Peek((char*)&header, sizeof(PacketHeader));

		// 3. 헤더의 code 확인
		if ((unsigned char)header.byCode != (unsigned char)0x89) {
			Disconnect(pSession);
			return;
		}
		// 4. 헤더의 len 값과 recq 데이터 사이즈 비교
		// 완성 패킷  사이즈 : 헤더 + len , 아직 덜오면 넘김. 다음루프에 처리
		if (pSession->RecvQ.GetUseSize() < sizeof(PacketHeader) + header.bySize)	break;


		// 5. 데이터 peek 했던 헤더를 recvq에서 지우고 (또 뺄 필요없음. 이미 뺐으니)
		pSession->RecvQ.MoveFront(sizeof(PacketHeader));

		// Dequeue 없이 직접 포인터 넘기기
		if (pSession->RecvQ.DirectDequeueSize() >= header.bySize) {
			PacketProc(pSession, header.byType, pSession->RecvQ.GetFrontBufferPtr());
			pSession->RecvQ.MoveFront(header.bySize);
		}
		else {
			// 쪼개진 경우 어쩔 수 없이 로컬버퍼 사용
			pSession->RecvQ.Dequeue(chTemp, header.bySize);
			PacketProc(pSession, header.byType, chTemp);
		}

	}


}

void netProc_Send(st_SESSION* pSession) {

	// 송신 버퍼 여유 생김 (못 보낸 거 이어서 전송)
	if (pSession->SendQ.GetUseSize() == 0) {
		// 보낼게 없음.
		return;
	}
	int sendSize = pSession->SendQ.DirectDequeueSize();

	int retSend = send(pSession->Socket, pSession->SendQ.GetFrontBufferPtr(), sendSize, 0);

	if (retSend == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			// 디스 커넥트
			Disconnect(pSession);
		}
	}
	else {
		pSession->SendQ.MoveFront(retSend);
	}

}

bool PacketProc(st_SESSION* pSession, BYTE byPacketType, char* pPacket) {
	switch (byPacketType) {

	case dfPACKET_CS_MOVE_START:
		return netPacketProc_MoveStart(pSession, pPacket);
		break;
	case dfPACKET_CS_MOVE_STOP:
		return netPacketProc_MoveStop(pSession, pPacket);
		break;
	case dfPACKET_CS_ATTACK1:
		return netPacketProc_Attack1(pSession, pPacket);
		break;
	case dfPACKET_CS_ATTACK2:
		return netPacketProc_Attack2(pSession, pPacket);
		break;
	case dfPACKET_CS_ATTACK3:
		return netPacketProc_Attack3(pSession, pPacket);
		break;
	}

	return TRUE;
}


void Disconnect(st_SESSION* PlayerID) {
	// 중복 방지
	if (PlayerID->bDisconnect) return;
	PacketDELETE Packet{};
	PacketHeader Header = CreatePacketHeader();
	CreateDeletePacketPlayer(&Header, &Packet, PlayerID->dwSessionID);

	// 대상 Player 제거 erase 여기서 문제. 지연삭제 예정
	PlayerID->bDisconnect = true;
	SendBroadcast(PlayerID, &Header, &Packet);

}

void SendUnicast(st_SESSION* PlayerID, PacketHeader* Header, void* Packet) {

	int Size = Header->bySize;
	// 해당 인큐가 버퍼 짤리면 링버퍼가 다 찬 거니 끊어버림.

	// 헤더 인큐
	if (PlayerID->SendQ.Enqueue((char*)Header, sizeof(PacketHeader)) != sizeof(PacketHeader)) {
		Disconnect(PlayerID);
		return;
	}
	// 보낼 데이터 인큐
	if (PlayerID->SendQ.Enqueue((char*)Packet, Size) != Size) {
		Disconnect(PlayerID);
		return;
	}

}

void SendBroadcast(st_SESSION* ExceptID, PacketHeader* Header, void* Packet) {
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		if (ExceptID != &(*it) && !(*it).bDisconnect) {
			SendUnicast(&(*it), Header, Packet);
		}
	}
}

void CleanupDisconnected() {

	st_SESSION* prev = nullptr;
	st_SESSION* cur = g_PlayerList.head;
	while (cur != nullptr) {
		if (cur->bDisconnect) {
			closesocket(cur->Socket);
			if (prev == nullptr)
				g_PlayerList.head = cur->next;
			else
				prev->next = cur->next;
			st_SESSION* del = cur;
			cur = cur->next;
			delete del;
		}
		else {
			prev = cur;
			cur = cur->next;
		}
	}
}

bool netPacketProc_MoveStart(st_SESSION* pSession, char* pPacket) {
	Packet_CS_Move_Start* pMove = (Packet_CS_Move_Start*)pPacket;
	pSession->byDirection = pMove->Direction;

	// 오차 범위 무시하기
	if (abs(pSession->shX - pMove->X) > dfERROR_RANGE ||
		abs(pSession->shY - pMove->Y) > dfERROR_RANGE)
	{
		Disconnect(pSession);
		// 로그출력
		wprintf(L"오차범위 초과 - 서버X:%d 클X:%d 서버Y:%d 클Y:%d\n",
			pSession->shX, pMove->X, pSession->shY, pMove->Y);
		return false;
	}

	//-----------------------------------------------------
	// 동작을 변경. 지금 구현에선 동작번호가 방향값이다
	//-----------------------------------------------------
	pSession->dwAction = pMove->Direction;

	//dfERROR_RANGE
	pSession->shX = pMove->X;
	pSession->shY = pMove->Y;


	PacketHeader Header = CreatePacketHeader();
	Packet_SC_Move_Start Packet{};
	CreatePacketSCMoveStart(&Header, &Packet, pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);
	// 센드 브로드 캐스트 (나 제외)
	SendBroadcast(pSession, &Header, &Packet);

	// netPacketProc_MoveStart
	wprintf(L"# PACKET_MOVESTART # SessionID:%d / Direction:%d / X:%d / Y:%d\n", pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);


	return true;
}
bool netPacketProc_MoveStop(st_SESSION* pSession, char* pPacket) {
	Packet_CS_Move_Stop* pStop = (Packet_CS_Move_Stop*)pPacket;


	// 오차 범위 무시하기
	if (abs(pSession->shX - pStop->X) > dfERROR_RANGE ||
		abs(pSession->shY - pStop->Y) > dfERROR_RANGE)
	{
		Disconnect(pSession);
		// 로그출력

		wprintf(L"오차범위 초과 - 서버X:%d 클X:%d 서버Y:%d 클Y:%d\n",
			pSession->shX, pStop->X, pSession->shY, pStop->Y);
		return false;
	}


	//-----------------------------------------------------
	// 방향을 변경.
	//-----------------------------------------------------
	switch (pStop->Direction)
	{
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		pSession->byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LD:
		pSession->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	}

	//-----------------------------------------------------
	// 동작을 변경. 지금 구현에선 동작번호가 방향값이다
	//-----------------------------------------------------
	pSession->dwAction = dfACTION_STOP;



	pSession->shX = pStop->X;
	pSession->shY = pStop->Y;

	PacketHeader Header = CreatePacketHeader();
	Packet_SC_Move_Stop Packet{};
	CreatePacketSCMoveStop(&Header, &Packet, pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);
	// 센드 브로드 캐스트 (나 제외)
	SendBroadcast(pSession, &Header, &Packet);

	// netPacketProc_MoveStart
	wprintf(L"# PACKET_MOVESTOP # SessionID:%d / Direction:%d / X:%d / Y:%d\n", pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);


	return true;
}
bool netPacketProc_Attack1(st_SESSION* pSession, char* pPacket) {

	Packet_CS_ATTACK1* pAtk = (Packet_CS_ATTACK1*)pPacket;

	// 오차 범위 체크
	if (abs(pSession->shX - pAtk->X) > dfERROR_RANGE ||
		abs(pSession->shY - pAtk->Y) > dfERROR_RANGE) {
		Disconnect(pSession);
		return false;
	}

	pSession->shX = pAtk->X;
	pSession->shY = pAtk->Y;

	// 좌/우만 사용
	switch (pAtk->Direction) {
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		pSession->byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	default:
		pSession->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	}

	PacketHeader Header = CreatePacketHeader();
	Packet_SC_ATTACK1 Packet{};
	CreatePacketSCAttack1(&Header, &Packet, pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);
	SendBroadcast(pSession, &Header, &Packet);

	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pTarget = &(*it);
		if (pTarget == pSession || pTarget->bDisconnect) continue;

		// 범위 체크 (Attack1 범위는 프로토콜/기획 따라 조정)
		if (pSession->byDirection == dfPACKET_MOVE_DIR_RR) {
			if (pTarget->shX - pSession->shX >= 0 &&
				pTarget->shX - pSession->shX < dfATTACK1_RANGE_X &&
				abs(pTarget->shY - pSession->shY) < dfATTACK1_RANGE_Y) {
				pTarget->chHP -= 10;
				PacketHeader DmgHeader = CreatePacketHeader();
				Packet_SC_Damage DmgPacket{};
				CreatePacketSCDamage(&DmgHeader, &DmgPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP);
				//직렬화 버퍼 넣는다면, 코드 텍스트기로 만든 함수 호출
				// CreatePacketSCDamge(&CPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP );
				// SendPacket(CPacket);

				//Item 자료형이라면? CPacket << Item; (id, x,y)
				// 원래라면 , cPacket << cnt 후에 cPacket << Item cnt만큼 반복
				// 이거 조차 ItemList로 만들어서 cPacket << ItemList 가능 

				SendBroadcast(nullptr, &DmgHeader, &DmgPacket);
				if (pTarget->chHP <= 0)
					Disconnect(pTarget);
			}
		}
		else {
			if (pSession->shX - pTarget->shX >= 0 &&
				pSession->shX - pTarget->shX < dfATTACK1_RANGE_X &&
				abs(pTarget->shY - pSession->shY) < dfATTACK1_RANGE_Y) {
				pTarget->chHP -= 10;
				PacketHeader DmgHeader = CreatePacketHeader();
				Packet_SC_Damage DmgPacket{};
				CreatePacketSCDamage(&DmgHeader, &DmgPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP);
				SendBroadcast(nullptr, &DmgHeader, &DmgPacket);
				if (pTarget->chHP <= 0)
					Disconnect(pTarget);
			}
		}

	}


	return true;
}
bool netPacketProc_Attack2(st_SESSION* pSession, char* pPacket) {
	Packet_CS_ATTACK2* pAtk = (Packet_CS_ATTACK2*)pPacket;
	// 오차 범위 체크
	if (abs(pSession->shX - pAtk->X) > dfERROR_RANGE ||
		abs(pSession->shY - pAtk->Y) > dfERROR_RANGE) {
		Disconnect(pSession);
		return false;
	}
	pSession->shX = pAtk->X;
	pSession->shY = pAtk->Y;
	// 좌/우만 사용
	switch (pAtk->Direction) {
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		pSession->byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	default:
		pSession->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	}
	PacketHeader Header = CreatePacketHeader();
	Packet_SC_ATTACK2 Packet{};
	CreatePacketSCAttack2(&Header, &Packet, pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);
	SendBroadcast(pSession, &Header, &Packet);
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pTarget = &(*it);
		if (pTarget == pSession || pTarget->bDisconnect) continue;
		if (pSession->byDirection == dfPACKET_MOVE_DIR_RR) {
			if (pTarget->shX - pSession->shX >= 0 &&
				pTarget->shX - pSession->shX < dfATTACK2_RANGE_X &&
				abs(pTarget->shY - pSession->shY) < dfATTACK2_RANGE_Y) {
				pTarget->chHP -= 10;
				PacketHeader DmgHeader = CreatePacketHeader();
				Packet_SC_Damage DmgPacket{};
				CreatePacketSCDamage(&DmgHeader, &DmgPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP);
				SendBroadcast(nullptr, &DmgHeader, &DmgPacket);
				if (pTarget->chHP <= 0)
					Disconnect(pTarget);
			}
		}
		else {
			if (pSession->shX - pTarget->shX >= 0 &&
				pSession->shX - pTarget->shX < dfATTACK2_RANGE_X &&
				abs(pTarget->shY - pSession->shY) < dfATTACK2_RANGE_Y) {
				pTarget->chHP -= 10;
				PacketHeader DmgHeader = CreatePacketHeader();
				Packet_SC_Damage DmgPacket{};
				CreatePacketSCDamage(&DmgHeader, &DmgPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP);
				SendBroadcast(nullptr, &DmgHeader, &DmgPacket);
				if (pTarget->chHP <= 0)
					Disconnect(pTarget);
			}
		}
	}
	return true;
}

bool netPacketProc_Attack3(st_SESSION* pSession, char* pPacket) {
	Packet_CS_ATTACK3* pAtk = (Packet_CS_ATTACK3*)pPacket;
	// 오차 범위 체크
	if (abs(pSession->shX - pAtk->X) > dfERROR_RANGE ||
		abs(pSession->shY - pAtk->Y) > dfERROR_RANGE) {
		Disconnect(pSession);
		return false;
	}
	pSession->shX = pAtk->X;
	pSession->shY = pAtk->Y;
	// 좌/우만 사용
	switch (pAtk->Direction) {
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		pSession->byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	default:
		pSession->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	}
	PacketHeader Header = CreatePacketHeader();
	Packet_SC_ATTACK3 Packet{};
	CreatePacketSCAttack3(&Header, &Packet, pSession->dwSessionID, pSession->byDirection, pSession->shX, pSession->shY);
	SendBroadcast(pSession, &Header, &Packet);
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pTarget = &(*it);
		if (pTarget == pSession || pTarget->bDisconnect) continue;
		if (pSession->byDirection == dfPACKET_MOVE_DIR_RR) {
			if (pTarget->shX - pSession->shX >= 0 &&
				pTarget->shX - pSession->shX < dfATTACK3_RANGE_X &&
				abs(pTarget->shY - pSession->shY) < dfATTACK3_RANGE_Y) {
				pTarget->chHP -= 10;
				PacketHeader DmgHeader = CreatePacketHeader();
				Packet_SC_Damage DmgPacket{};
				CreatePacketSCDamage(&DmgHeader, &DmgPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP);
				SendBroadcast(nullptr, &DmgHeader, &DmgPacket);
				if (pTarget->chHP <= 0)
					Disconnect(pTarget);
			}
		}
		else {
			if (pSession->shX - pTarget->shX >= 0 &&
				pSession->shX - pTarget->shX < dfATTACK3_RANGE_X &&
				abs(pTarget->shY - pSession->shY) < dfATTACK3_RANGE_Y) {
				pTarget->chHP -= 10;
				PacketHeader DmgHeader = CreatePacketHeader();
				Packet_SC_Damage DmgPacket{};
				CreatePacketSCDamage(&DmgHeader, &DmgPacket, pSession->dwSessionID, pTarget->dwSessionID, pTarget->chHP);
				SendBroadcast(nullptr, &DmgHeader, &DmgPacket);
				if (pTarget->chHP <= 0)
					Disconnect(pTarget);
			}
		}
	}
	return true;
}