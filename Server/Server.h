#pragma once

#include "ringbuffer.h"
#include <Windows.h>

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

extern bool g_bShutdown;
extern SessionList g_PlayerList;
extern SOCKET g_ListenSocket;
extern DWORD g_ID;