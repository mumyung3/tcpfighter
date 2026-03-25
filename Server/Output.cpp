#include "common.h"

#include "output.h"

void Create_Packet_SC_Damage(CPacket* clpPacket, unsigned long AttackID, unsigned long DamageID, char DamageHP)
{
	*clpPacket << AttackID;
	*clpPacket << DamageID;
	*clpPacket << DamageHP;
}

void Parse_Packet_SC_Damage(CPacket* clpPacket, unsigned long& AttackID, unsigned long& DamageID, char& DamageHP)
{
	*clpPacket >> AttackID;
	*clpPacket >> DamageID;
	*clpPacket >> DamageHP;
}

void Create_Packet_SC_ATTACK3(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_SC_ATTACK3(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_CS_ATTACK3(CPacket* clpPacket, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_CS_ATTACK3(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_SC_ATTACK2(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_SC_ATTACK2(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_CS_ATTACK2(CPacket* clpPacket, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_CS_ATTACK2(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_SC_ATTACK1(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_SC_ATTACK1(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_CS_ATTACK1(CPacket* clpPacket, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_CS_ATTACK1(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_SC_Move_Stop(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_SC_Move_Stop(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_CS_Move_Stop(CPacket* clpPacket, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_CS_Move_Stop(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_SC_Move_Start(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_SC_Move_Start(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_Packet_CS_Move_Start(CPacket* clpPacket, char Direction, unsigned short X, unsigned short Y)
{
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
}

void Parse_Packet_CS_Move_Start(CPacket* clpPacket, char& Direction, unsigned short& X, unsigned short& Y)
{
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
}

void Create_PacketDELETE(CPacket* clpPacket, unsigned long ID)
{
	*clpPacket << ID;
}

void Parse_PacketDELETE(CPacket* clpPacket, unsigned long& ID)
{
	*clpPacket >> ID;
}

void Create_PacketCreateOtherPlayer(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y, char HP)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
	*clpPacket << HP;
}

void Parse_PacketCreateOtherPlayer(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y, char& HP)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
	*clpPacket >> HP;
}

void Create_PacketCreatePlayer(CPacket* clpPacket, unsigned long ID, char Direction, unsigned short X, unsigned short Y, char HP)
{
	*clpPacket << ID;
	*clpPacket << Direction;
	*clpPacket << X;
	*clpPacket << Y;
	*clpPacket << HP;
}

void Parse_PacketCreatePlayer(CPacket* clpPacket, unsigned long& ID, char& Direction, unsigned short& X, unsigned short& Y, char& HP)
{
	*clpPacket >> ID;
	*clpPacket >> Direction;
	*clpPacket >> X;
	*clpPacket >> Y;
	*clpPacket >> HP;
}

