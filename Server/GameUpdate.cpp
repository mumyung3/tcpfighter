#include "GameUpdate.h"
#include "NetWorkIOProcess.h"

void UpdateGame() {
	// 50fps 타이밍 체크
	static DWORD dwPrevTime = timeGetTime();
	DWORD dwCurTime = timeGetTime();
	if (dwCurTime - dwPrevTime < 1000 / 50) return;
	dwPrevTime = dwCurTime;

	for (auto it = g_PlayerList.begin(); it != g_PlayerList.end(); ++it) {
		st_SESSION* pSession = &(*it);

		if (pSession->chHP <= 0) {
			Disconnect(pSession);
			continue;
		}

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

		// 맵 경계 처리
		if (pSession->shX < dfRANGE_MOVE_LEFT)   pSession->shX = dfRANGE_MOVE_LEFT;
		if (pSession->shX > dfRANGE_MOVE_RIGHT)  pSession->shX = dfRANGE_MOVE_RIGHT;
		if (pSession->shY < dfRANGE_MOVE_TOP)    pSession->shY = dfRANGE_MOVE_TOP;
		if (pSession->shY > dfRANGE_MOVE_BOTTOM) pSession->shY = dfRANGE_MOVE_BOTTOM;
	}
}