#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#include <tchar.h>
#include <Windows.h>
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

#include "ringbuffer.h"
//#include "NetWorkIOProcess.h"
#include "locale.h"

#include "GameUpdate.h"

#define SERVERPORT 5000
#define SERVERIP L"192.168.10.100"

extern bool g_bShutdown;


// 세션 + 플레이어 객체
struct st_SESSION {

	SOCKET Socket;		// 소켓
	DWORD dwSessionID;	// 고유 세션 id
	RingBuffer RecvQ;	// 송수신 큐
	RingBuffer SendQ;

	DWORD dwAction;

	short shX;
	short shY;

	st_SESSION* next;

	char chHP;
	bool bDisconnect{};

	WCHAR   ip[16]{};
	unsigned short port;
	BYTE byDirection;
};

struct SessionList {
	st_SESSION* head;

	struct Iterator {
		st_SESSION* ptr;
		Iterator& operator++() { ptr = ptr->next; return *this; }
		st_SESSION& operator*() { return *ptr; }
		bool operator!=(const Iterator& other) { return ptr != other.ptr; }
	};

	Iterator begin() { return { head }; }
	Iterator end() { return { nullptr }; }
};


extern SessionList g_PlayerList;

extern SOCKET g_ListenSocket;

// 세션 id	 고유
extern DWORD g_id;


void cs_Initial(void);
void cs_MoveCursor(int iPosX, int iPosY);