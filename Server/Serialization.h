#pragma once

//#define DEBUG_SERIALIZE

#ifdef DEBUG_SERIALIZE
extern wchar_t g_function[64];
#endif

class CPacket {

	enum en_PACKET {

		MAXMESSAGESIZE = 150
	};

public:

	CPacket();
	CPacket(int bufferSize);

	virtual ~CPacket();

	CPacket& operator=(const CPacket& packet) = delete;
	CPacket(const CPacket& Packet) = delete;

	char* buffer;
	int front;
	int rear;
	bool bError;
	int bufferSize;
	int dataSize;
	bool IsEmpty();
	bool IsFull();
	int GetUseSize();
	int GetRemainSize();
	int GetBufferSize();
	void Clear();
	char* GetBufferPtr();
	int MoveWritePos(int iSize);
	int MoveReadPos(int iSize);

	CPacket& operator<<(const unsigned char& byValue);
	CPacket& operator<<(const char& chValue);
	CPacket& operator<<(const short& shValue);
	CPacket& operator<<(const unsigned short& wValue);
	CPacket& operator<<(const int& iValue);
	CPacket& operator<<(const long& lValue);
	CPacket& operator<<(const float& fValue);
	CPacket& operator<<(const __int64& iValue);
	CPacket& operator<<(const double& dValue);
	int PutData(char* chpSrc, int iSize);

	CPacket& operator>>(unsigned char& byValue);
	CPacket& operator>>(char& chValue);
	CPacket& operator>>(short& shValue);
	CPacket& operator>>(unsigned short& wValue);
	CPacket& operator>>(int& iValue);
	CPacket& operator>>(long& lValue);
	CPacket& operator>>(float& fValue);
	CPacket& operator>>(__int64& iValue);
	CPacket& operator>>(double& dValue);
	int GetData(char* chpDest, int iSize);

};