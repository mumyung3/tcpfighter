
#include "Serialization.h"
#include <iostream>
#include <Windows.h>


#ifdef DEBUG_SERIALIZE
wchar_t g_function[64];
#endif

CPacket::CPacket() : front(0), rear(0), bError(false), bufferSize(MAXMESSAGESIZE), dataSize(0) {
	buffer = (char*)malloc(MAXMESSAGESIZE);
	if (buffer == nullptr) {
		bError = true;
		int errorcode = GetLastError();
		__debugbreak();
		return;
	}
	memset(buffer, 0, MAXMESSAGESIZE);
}

CPacket::CPacket(int bufferSize) : front(0), rear(0), bError(false), bufferSize(bufferSize), dataSize(0) {
	buffer = (char*)malloc(bufferSize);
	if (buffer == nullptr) {
		bError = true;
		int errorcode = GetLastError();
		__debugbreak();
		return;
	}
	memset(buffer, 0, bufferSize);
}

CPacket::~CPacket() {
	free(buffer);
}


bool CPacket::IsEmpty() {
	if (rear == front) {
		return true;
	}

	return false;

}

bool CPacket::IsFull() {
	if (rear == bufferSize) {
		return true;
	}
	return false;
}
int CPacket::GetUseSize() {
	dataSize = rear - front;
	return dataSize;
}

int CPacket::GetRemainSize() {
	return bufferSize - rear;
}

void CPacket::Clear() {
	memset(buffer, 0, bufferSize);
	front = 0;
	rear = 0;
	dataSize = 0;
	bError = false;
}

int CPacket::GetBufferSize() {
	return bufferSize;
}
char* CPacket::GetBufferPtr() {
	return buffer;
}

int CPacket::MoveWritePos(int iSize) {
	if (GetRemainSize() < iSize)	return -1;
	rear += iSize;

	return iSize;
}
int CPacket::MoveReadPos(int iSize) {
	if (GetUseSize() < iSize)	return -1;
	front += iSize;
	return iSize;

}

CPacket& CPacket::operator<<(const unsigned char& byValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(byValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &byValue, sizeof(byValue));
	rear += sizeof(byValue);
	return *this;
}
CPacket& CPacket::operator<<(const char& chValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(chValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &chValue, sizeof(chValue));
	rear += sizeof(chValue);
	return *this;
}
CPacket& CPacket::operator<<(const short& shValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(shValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &shValue, sizeof(shValue));
	rear += sizeof(shValue);
	return *this;
}
CPacket& CPacket::operator<<(const unsigned short& wValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(wValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &wValue, sizeof(wValue));
	rear += sizeof(wValue);
	return *this;
}
CPacket& CPacket::operator<<(const int& iValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(iValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &iValue, sizeof(iValue));
	rear += sizeof(iValue);
	return *this;
}
CPacket& CPacket::operator<<(const long& lValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(lValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &lValue, sizeof(lValue));
	rear += sizeof(lValue);
	return *this;
}
CPacket& CPacket::operator<<(const float& fValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(fValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &fValue, sizeof(fValue));
	rear += sizeof(fValue);
	return *this;
}
CPacket& CPacket::operator<<(const __int64& iValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(iValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &iValue, sizeof(iValue));
	rear += sizeof(iValue);
	return *this;
}
CPacket& CPacket::operator<<(const double& dValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < sizeof(dValue)) { bError = true; return *this; }
	memcpy(buffer + rear, &dValue, sizeof(dValue));
	rear += sizeof(dValue);
	return *this;
}

CPacket& CPacket::operator>>(unsigned char& byValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(byValue)) { bError = true; return *this; }
	memcpy(&byValue, buffer + front, sizeof(byValue));
	front += sizeof(byValue);
	return *this;
}

CPacket& CPacket::operator>>(char& chValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(chValue)) { bError = true; return *this; }
	memcpy(&chValue, buffer + front, sizeof(chValue));
	front += sizeof(chValue);
	return *this;
}
CPacket& CPacket::operator>>(short& shValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(shValue)) { bError = true; return *this; }
	memcpy(&shValue, buffer + front, sizeof(shValue));
	front += sizeof(shValue);
	return *this;
}
CPacket& CPacket::operator>>(unsigned short& wValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(wValue)) { bError = true; return *this; }
	memcpy(&wValue, buffer + front, sizeof(wValue));
	front += sizeof(wValue);
	return *this;
}
CPacket& CPacket::operator>>(int& iValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(iValue)) { bError = true; return *this; }
	memcpy(&iValue, buffer + front, sizeof(iValue));
	front += sizeof(iValue);
	return *this;
}
CPacket& CPacket::operator>>(long& lValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(lValue)) { bError = true; return *this; }
	memcpy(&lValue, buffer + front, sizeof(lValue));
	front += sizeof(lValue);
	return *this;
}
CPacket& CPacket::operator>>(float& fValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(fValue)) { bError = true; return *this; }
	memcpy(&fValue, buffer + front, sizeof(fValue));
	front += sizeof(fValue);
	return *this;
}
CPacket& CPacket::operator>>(__int64& iValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(iValue)) { bError = true; return *this; }
	memcpy(&iValue, buffer + front, sizeof(iValue));
	front += sizeof(iValue);
	return *this;
}
CPacket& CPacket::operator>>(double& dValue) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < sizeof(dValue)) { bError = true; return *this; }
	memcpy(&dValue, buffer + front, sizeof(dValue));
	front += sizeof(dValue);
	return *this;
}

// 삽입
int CPacket::PutData(char* chpSrc, int iSize) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetRemainSize() < iSize) { bError = true; return -1; }
	memcpy(buffer + rear, chpSrc, iSize);
	rear += iSize;
	return iSize;
}

// 얻기
int CPacket::GetData(char* chpDest, int iSize) {

#ifdef DEBUG_SERIALIZE
	swprintf_s(g_function, sizeof(g_function) / sizeof(wchar_t), L"%S", __FUNCTION__);
#endif

	if (GetUseSize() < iSize) { bError = true; return -1; }
	memcpy(chpDest, buffer + front, iSize);
	front += iSize;
	return iSize;
}