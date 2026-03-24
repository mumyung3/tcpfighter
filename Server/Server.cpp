#include "Server.h"
#include <Windows.h>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")
#include "NetWorkIOProcess.h"
#include "GameUpdate.h"
#include "locale.h"
#include <tchar.h>

#define SERVERPORT 5000
#define SERVERIP L"192.168.10.100"

bool g_bShutdown = false;

SOCKET g_ListenSocket{};

SessionList g_PlayerList{};	//네트워크, 게임업데이트에서 필요함.

DWORD g_ID = 0;

HANDLE  hConsole;

int _tmain(int argc, _TCHAR* argv[])
{
	timeBeginPeriod(1);
	_wsetlocale(LC_ALL, L"Korean");

	DWORD StartTime = timeGetTime();

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		wprintf(L"WSAStartup 실패\n");
		return 1;
	}

	g_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (g_ListenSocket == INVALID_SOCKET) {
		int errorCode = WSAGetLastError();
		wprintf(L"socket 실패 : %d\n", errorCode);
		return 1;
	}

	// non-block
	u_long mode = 1;
	ioctlsocket(g_ListenSocket, FIONBIO, &mode);
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVERPORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	// bind()
	if (bind(g_ListenSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		wprintf(L"bind 실패 : %d\n", errorCode);
		return 1;
	}
	// listen()
	if (listen(g_ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		int errorCode = WSAGetLastError();
		wprintf(L"listen 실패 : %d\n", errorCode);
		return 1;
	}

	wprintf(L"%40s\n", L"**서버 시작**");

	while (!g_bShutdown) {
		// 네트워크 송수신 처리
		NetIOProcess();
		// 게임 로직 업데이트
		UpdateGame();

#pragma region FPS
		DWORD EndTime = timeGetTime();

		int Interval = 20 - (EndTime - StartTime);
		if (Interval > 0)
			Sleep(Interval);

		StartTime += 20;
#pragma endregion

	}

	closesocket(g_ListenSocket);
	WSACleanup();

	// 해제 처리 해주기. 차후

	return 0;
}
