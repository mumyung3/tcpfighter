#include "GameUpdate.h"
#include "Server.h"
#include "common.h"
#include "NetWorkIOProcess.h"
#include <stdio.h>
void UpdateGame() {
	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pSession = &(*it);

		if (pSession->chHP <= 0) {
			Disconnect(pSession);
			continue;
		}

		short prevX = pSession->shX;
		short prevY = pSession->shY;


		switch (pSession->dwAction) {
		case dfPACKET_MOVE_DIR_LL:
			pSession->shX -= dfSPEED_X;

			break;
		case dfPACKET_MOVE_DIR_LU:
			pSession->shX -= dfSPEED_X;
			pSession->shY -= dfSPEED_Y;

			break;
		case dfPACKET_MOVE_DIR_UU:
			pSession->shY -= dfSPEED_Y;
			break;
		case dfPACKET_MOVE_DIR_RU:
			pSession->shX += dfSPEED_X;
			pSession->shY -= dfSPEED_Y;
			break;
		case dfPACKET_MOVE_DIR_RR:
			pSession->shX += dfSPEED_X;
			break;
		case dfPACKET_MOVE_DIR_RD:
			pSession->shX += dfSPEED_X;
			pSession->shY += dfSPEED_Y;
			break;
		case dfPACKET_MOVE_DIR_DD:
			pSession->shY += dfSPEED_Y;
			break;
		case dfPACKET_MOVE_DIR_LD:
			pSession->shX -= dfSPEED_X;
			pSession->shY += dfSPEED_Y;
			break;
		}


		if (pSession->shX < dfRANGE_MOVE_LEFT || pSession->shX > dfRANGE_MOVE_RIGHT)
			pSession->shY = prevY;  // X가 벽 닿으면 Y 되돌림
		if (pSession->shY < dfRANGE_MOVE_TOP || pSession->shY > dfRANGE_MOVE_BOTTOM)
			pSession->shX = prevX;  // Y가 벽 닿으면 X 되돌림


		// 맵 경계 처리
		if (pSession->shX < dfRANGE_MOVE_LEFT)	pSession->shX = dfRANGE_MOVE_LEFT;
		if (pSession->shX > dfRANGE_MOVE_RIGHT)  pSession->shX = dfRANGE_MOVE_RIGHT;
		if (pSession->shY < dfRANGE_MOVE_TOP)    pSession->shY = dfRANGE_MOVE_TOP;
		if (pSession->shY > dfRANGE_MOVE_BOTTOM) pSession->shY = dfRANGE_MOVE_BOTTOM;

		//// UpdateGame 이동 처리 후
		if (pSession->dwAction < 8) {
			const wchar_t* dirStr[] = { L"LL", L"LU", L"UU", L"RU", L"RR", L"RD", L"DD", L"LD" };
			wprintf(L"# gameRun:%s # SessionID:%d / X:%d / Y:%d\n", dirStr[pSession->dwAction], pSession->dwSessionID, pSession->shX, pSession->shY);
		}
	}
}