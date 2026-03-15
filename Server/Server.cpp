#include "Server.h"
#include "NetWorkIOProcess.h"

// 전역
bool g_bShutdown = false;


SOCKET g_ListenSocket{};

// 세션 LIST 
SessionList g_PlayerList{};
// 세션 id	 고유
DWORD g_id = 0;

HANDLE  hConsole;

int _tmain(int argc, _TCHAR* argv[])
{
	//초기화 ();
	timeBeginPeriod(1);
	//cs_Initial();
	DWORD StartTime = timeGetTime();

	// 1초를 재기 위한 시간

	DWORD OldTime = StartTime;

	// 렌더링 할지말지에 대한 상태 저장

	bool bIsRendering = true;

	_wsetlocale(LC_ALL, L"Korean");

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		wprintf(L"WSAStartup 실패\n");
		return 1;
	}


	// 서버 listsen 함수 호출 
	g_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (g_ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket 실패 : %d\n", WSAGetLastError());
		return 1;
	}

	// 논 블로킹
	u_long mode = 1;
	ioctlsocket(g_ListenSocket, FIONBIO, &mode);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVERPORT);
	//InetPton(AF_INET, SERVERIP, &addr.sin_addr);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(g_ListenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		wprintf(L"bind 실패 : %d\n", WSAGetLastError());
		return 1;
	}

	if (listen(g_ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		wprintf(L"listen 실패 : %d\n", WSAGetLastError());
		return 1;
	}

	wprintf(L"서버 시작\n");



	while (!g_bShutdown) {
		// 네트워크 송수신 처리
		NetIOProcess();
		// 게임 로직 업데이트
		UpdateGame();

		// 프레임 맞추기용 대기 Sleep(X)
#pragma region FPS

		// 한바퀴 프레임 길이 체크 
		DWORD EndTime = timeGetTime();


		// 프레임 출력
		static int Counter = 0;
		static int FramePerSeconds = 0;
		Counter++;
		if (EndTime - OldTime > 1000) {
			FramePerSeconds = Counter;
			Counter = 0;
			OldTime += 1000;

		}

		//프레임 출력
		//cs_MoveCursor(0, 0);
		//printf("FPS : %d", FramePerSeconds);


		// 느린 프레임을 위해 렌더링 건너뜀
		if ((StartTime + 20 + 20) > EndTime) {
			// 렌더링 하기
			bIsRendering = true;
		}
		else {
			// 다음 렌더링 스킵하기
			bIsRendering = false;
		}


		// 빠른 프레임을 늦추기 위해 쉼.(Sleep)
		int Interval = 20 - (EndTime - StartTime);
		if (Interval > 0)
			Sleep(Interval);

		StartTime += 20;

#pragma endregion

	}


	// 서버 종료 대기. 

	// 리슨 소켓 해제
	closesocket(g_ListenSocket);
	WSACleanup();

	return 0;
}
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	//-------------------------------------------------------------
	// 화면의 커서를 안보이게끔 설정한다.
	//-------------------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;


	//-------------------------------------------------------------
	// 콘솔화면 (스텐다드 아웃풋) 핸들을 구한다.
	//-------------------------------------------------------------
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	//-------------------------------------------------------------
	// 원하는 위치로 커서를 이동시킨다.
	//-------------------------------------------------------------
	SetConsoleCursorPosition(hConsole, stCoord);
}