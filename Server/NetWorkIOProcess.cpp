#include "NetWorkIOProcess.h"

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

	int iResult, iCnt;
	bool bProcFlag;

	Time.tv_sec = 0;
	Time.tv_usec = 0;
	iResult = select(0, &ReadSet, &WriteSet, 0 ,&Time);

	if (iResult > 0) {
		if (FD_ISSET(g_ListenSocket, &ReadSet)) {
			netProc_Accept();
		}

		// 전체 세션 반응 확인
		for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
			st_SESSION* pSession = &(*it);
			if (FD_ISSET(pSession->Socket, &ReadSet)) {
				--iResult;
				//netProc_Recv(pSession);
			}

			if (FD_ISSET(pSession->Socket, &WriteSet)) {
				--iResult;
				//netProc_Send(pSession);
			}

			//지연 삭제
			CleanupDisconnected();
		}
	}
	else if (iResult == SOCKET_ERROR) {
		
		int errCode = WSAGetLastError();
		g_bShutdown = true;

		wprintf(L"select 실패 : %d\n", errCode);
		__debugbreak();
		return;
	}
	
}

void netProc_Accept() {
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSock = accept(g_ListenSocket, (SOCKADDR*)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET) {
		wprintf(L"accept 실패 : %d\n", WSAGetLastError());
		g_bShutdown = true;
		return;
	}

	// g_playerlist 추가
	st_SESSION* newSession = new st_SESSION();
	newSession->Socket = clientSock;
	newSession->dwSessionID = g_id++;

	// 플레이어 생성 패킷 생성
	PacketCreatePlayer Packet{};
	PacketHeader Header = CreatePacketHeader();
	CreatePacketPlayer(&Header, &Packet);
	SendUnicast(newSession,&Header, (void*) & Packet);

	InetNtopW(AF_INET, &clientAddr.sin_addr, newSession->ip, 16);
	newSession->port = ntohs(clientAddr.sin_port);


	newSession->bDisconnect = false;
	newSession->next = g_PlayerList.head;
	g_PlayerList.head = newSession;

	wprintf(L"클라 접속 : IP=%s PORT=%d ID=%d\n", newSession->ip, newSession->port, newSession->dwSessionID);



}

void netProc_Recv(st_SESSION* pSession) {

	// 임시 수신 버퍼 선언 최대 10000 바이트ㅡ
	char chTemp[TEMP_BUFSIZE] = {0,};

	//recv 호출
	
	//recv 리턴값으로 종료 / 에러 socket 에러 처리
	
	// 데이터 받았다면 일단 recvq 인큐

	// 완료 패킷 처리
	while (1) {
		// 1. recvq에 최소한의 사이즈 확인 - 헤더 사이즈 초과
		// 2. recvq에서 헤더 peek
		// 3. 헤더의 code 확인
		// 4. 헤더의 len 값과 recq 데이터 사이즈 비교
		// 완성 패킷  사이즈 : 헤더 + len
		// 5. 데이터 peek 했던 헤더를 recvq에서 지우고 (또 뺄 필요없음. 이미 뺐으니)
		// 6. recq 에서 len 만큼 임시 패킷 버퍼로 뽑음.
		// 7. 헤더의 타입에 따른 분기를 위해 패킷 프로시저 호출

		//PacketProc(세션, 헤더의 타입, 패킷버퍼);
	}


}
//
//bool PacketProc(st_SESSION* pSession, BYTE byPacketType, char* pPacket) {
//	switch (byPacketType) {
//	
//	case dfPACKET_CS_MOVE_START:
//		return netPacketProc_MoveStart(pSession, pPacket);
//		break;
//	case dfPACKET_CS_MOVE_STOP:
//		return netPacketProc_MoveStop(pSession, pPacket);
//		break;
//	case dfPACKET_CS_ATTACK1:
//		return netPacketProc_Attack1(pSession, pPacket);
//		break;
//	case dfPACKET_CS_ATTACK2:
//		return netPacketProc_Attack2(pSession, pPacket);
//		break;
//	case dfPACKET_CS_ATTACK3:
//		return netPacketProc_Attack3(pSession, pPacket);
//		break;
//	}
//
//	return TRUE;
//}


void Disconnect(st_SESSION* PlayerID) {
	// 중복 방지
	if (PlayerID->bDisconnect) return;
	PacketDELETE Packet{};
	PacketHeader Header = CreatePacketHeader();
	CreateDeletePacketPlayer(&Header,&Packet, PlayerID->dwSessionID);

	// 대상 Player 제거 erase 여기서 문제. 지연삭제 예정
	PlayerID->bDisconnect = true;
	SendBroadcast(PlayerID, &Header,&Packet);

}

void SendUnicast(st_SESSION* PlayerID, PacketHeader* Header, void* Packet) {

	int Size = Header->bySize;
	// 헤더 인큐
	PlayerID->SendQ.Enqueue((char*)Header, sizeof(PacketHeader));

	//보낼 데이터 인큐
	PlayerID->SendQ.Enqueue((char*)Packet, Size);


}

void SendBroadcast(st_SESSION* ExceptID, PacketHeader* Header, void* Packet) {
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		if (ExceptID != &(*it) && !(*it).bDisconnect) {
			SendUnicast(&(*it), Header , Packet);
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