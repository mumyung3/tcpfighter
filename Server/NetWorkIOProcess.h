#pragma once
#include "common.h"
#include "Server.h"

void NetIOProcess();

void netProc_Accept();

void netProc_Recv(st_SESSION* pSession);
void netProc_Send(st_SESSION* pSession);


bool PacketProc(st_SESSION* pSession, BYTE byPacketType, char* pPacket);

void Disconnect(st_SESSION* PlayerID);
void SendUnicast(st_SESSION* PlayerID, PacketHeader* Header, void* Packet);

void SendBroadcast(st_SESSION* ExceptID, PacketHeader* Header, void* Packet);
void CleanupDisconnected();

bool netPacketProc_MoveStart(st_SESSION* pSession, char* pPacket);
bool netPacketProc_MoveStop(st_SESSION* pSession, char* pPacket);
bool netPacketProc_Attack1(st_SESSION* pSession, char* pPacket);
bool netPacketProc_Attack2(st_SESSION* pSession, char* pPacket);
bool netPacketProc_Attack3(st_SESSION* pSession, char* pPacket);