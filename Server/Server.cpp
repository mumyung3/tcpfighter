#include "Server.h"
#include "NetWorkIOProcess.h"

// 전역
bool g_bShutdown = false;


SOCKET g_ListenSocket{};

// 세션 LIST 
SessionList g_PlayerList{};
// 세션 id	 고유
DWORD g_id = 0;

int _tmain(int argc, _TCHAR * argv[])
{
	//초기화 ();
	timeBeginPeriod(1);
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
	}


	// 서버 종료 대기. 
	
	// 리슨 소켓 해제
	closesocket(g_ListenSocket);
	WSACleanup();

	return 0;
}