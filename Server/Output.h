#pragma once
#include "Serialization.h"

void Create_Packet_SC_Damage(CPacket * clpPacket, unsigned long AttackID, unsigned long DamageID, char DamageHP);
void Parse_Packet_SC_Damage(CPacket* clpPacket, unsigned long& AttackID, unsigned long& DamageID, char& DamageHP);
void Create_Packet_SC_ATTACK3(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_SC_ATTACK3(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_CS_ATTACK3(CPacket * clpPacket, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_CS_ATTACK3(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_SC_ATTACK2(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_SC_ATTACK2(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_CS_ATTACK2(CPacket * clpPacket, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_CS_ATTACK2(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_SC_ATTACK1(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_SC_ATTACK1(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_CS_ATTACK1(CPacket * clpPacket, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_CS_ATTACK1(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_SC_Move_Stop(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_SC_Move_Stop(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_CS_Move_Stop(CPacket * clpPacket, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_CS_Move_Stop(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_SC_Move_Start(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_SC_Move_Start(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y);
void Create_Packet_CS_Move_Start(CPacket * clpPacket, char Direction, unsigned short X, unsigned short Y);
void Parse_Packet_CS_Move_Start(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y);
void Create_PacketDELETE(CPacket * clpPacket, unsigned long ID);
void Parse_PacketDELETE(CPacket* clpPacket, unsigned long& ID);
void Create_PacketCreateOtherPlayer(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y, char HP);
void Parse_PacketCreateOtherPlayer(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y, char& HP);
void Create_PacketCreatePlayer(CPacket * clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y, char HP);
void Parse_PacketCreatePlayer(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y, char& HP);
