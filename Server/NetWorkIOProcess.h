#pragma once
#define TEMP_BUFSIZE 10000

#define dfRANGE_MOVE_TOP	50
#define dfRANGE_MOVE_LEFT	10
#define dfRANGE_MOVE_RIGHT	630
#define dfRANGE_MOVE_BOTTOM	470

//-----------------------------------------------------------------
// 이동 오류체크 범위
//-----------------------------------------------------------------
#define dfERROR_RANGE		50

//---------------------------------------------------------------
// 공격범위.
//---------------------------------------------------------------

#define dfATTACK1_RANGE_X		80
#define dfATTACK2_RANGE_X		90
#define dfATTACK3_RANGE_X		100
#define dfATTACK1_RANGE_Y		10
#define dfATTACK2_RANGE_Y		10
#define dfATTACK3_RANGE_Y		20

// # 프레임당 이동 단위-----------------------------------

// X 축 - 3
// Y 축 - 2


//#include "Server.h"
#include "PacketDefine.h"

void NetIOProcess();

void netProc_Accept();

void netProc_Recv(st_SESSION* pSession);

//bool PacketProc(st_SESSION* pSession, BYTE byPacketType, char* pPacket);

void Disconnect(st_SESSION* PlayerID);
void SendUnicast(st_SESSION* PlayerID, PacketHeader* Header, void* Packet);

void SendBroadcast(st_SESSION* ExceptID, PacketHeader* Header, void* Packet);
void CleanupDisconnected();