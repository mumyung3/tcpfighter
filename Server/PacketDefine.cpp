#include "PacketDefine.h"
#include "NetWorkIOProcess.h"

PacketHeader CreatePacketHeader() {
	PacketHeader Packet{};
	Packet.byCode = 0x89;

	// 미초기화
	Packet.bySize = -1;
	Packet.byType = -1;
	return Packet;
}

void CreatePacketPlayer(PacketHeader* Header, PacketCreatePlayer* Player) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(PacketCreatePlayer);
	Header->byType = dfPACKET_SC_CREATE_MY_CHARACTER;
	Player->ID = g_id;
	Player->Direction = rand() % 8;
	Player->X = dfRANGE_MOVE_LEFT + rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT + 1);
	Player->Y = dfRANGE_MOVE_TOP + rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP + 1);
	Player->HP = 100;
}

void CreatePacketOtherPlayer(PacketHeader* Header, PacketCreateOtherPlayer* OtherPlayer, unsigned long ID, unsigned char Direction, unsigned short X, unsigned short Y, unsigned char HP) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(PacketCreateOtherPlayer);
	Header->byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

	OtherPlayer->ID = ID;
	OtherPlayer->Direction = Direction;
	OtherPlayer->X = X;
	OtherPlayer->Y = Y;
	OtherPlayer->HP = HP;
}

void CreateDeletePacketPlayer(PacketHeader* Header, PacketDELETE* Player, unsigned long PlayerID) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(PacketDELETE);
	Header->byType = dfPACKET_SC_DELETE_CHARACTER;
	Player->ID = PlayerID;

}

void CreatePacketSCMoveStart(PacketHeader* Header, Packet_SC_Move_Start* Move, unsigned long ID, char Direction, unsigned short X, unsigned short Y) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(Packet_SC_Move_Start);
	Header->byType = dfPACKET_SC_MOVE_START;
	Move->ID = ID;
	Move->Direction = Direction;
	Move->X = X;
	Move->Y = Y;

}

void CreatePacketSCMoveStop(PacketHeader* Header, Packet_SC_Move_Stop* Move, unsigned long ID, char Direction, unsigned short X, unsigned short Y) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(Packet_SC_Move_Stop);
	Header->byType = dfPACKET_SC_MOVE_STOP;
	Move->ID = ID;
	Move->Direction = Direction;
	Move->X = X;
	Move->Y = Y;
}

void CreatePacketSCAttack1(PacketHeader* Header, Packet_SC_ATTACK1* Attack, unsigned long ID, char Direction, unsigned short X, unsigned short Y) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(Packet_SC_ATTACK1);
	Header->byType = dfPACKET_SC_ATTACK1;
	Attack->ID = ID;
	Attack->Direction = Direction;
	Attack->X = X;
	Attack->Y = Y;
}
void CreatePacketSCAttack2(PacketHeader* Header, Packet_SC_ATTACK2* Attack, unsigned long ID, char Direction, unsigned short X, unsigned short Y) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(Packet_SC_ATTACK2);
	Header->byType = dfPACKET_SC_ATTACK2;
	Attack->ID = ID;
	Attack->Direction = Direction;
	Attack->X = X;
	Attack->Y = Y;
}
void CreatePacketSCAttack3(PacketHeader* Header, Packet_SC_ATTACK3* Attack, unsigned long ID, char Direction, unsigned short X, unsigned short Y) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(Packet_SC_ATTACK3);
	Header->byType = dfPACKET_SC_ATTACK3;
	Attack->ID = ID;
	Attack->Direction = Direction;
	Attack->X = X;
	Attack->Y = Y;
}
void CreatePacketSCDamage(PacketHeader* Header, Packet_SC_Damage* Damage, unsigned long AttackID, unsigned long DamageID, char DamageHP) {
	Header->byCode = 0x89;
	Header->bySize = sizeof(Packet_SC_Damage);
	Header->byType = dfPACKET_SC_DAMAGE;
	Damage->AttackID = AttackID;
	Damage->DamageID = DamageID;
	Damage->DamageHP = DamageHP;
}