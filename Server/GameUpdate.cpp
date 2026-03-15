#include "GameUpdate.h"
#include "NetWorkIOProcess.h"

void UpdateGame() {
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

		// ¸Ê °æ°è Ã³¸®
		if (pSession->shX < dfRANGE_MOVE_LEFT)   pSession->shX = dfRANGE_MOVE_LEFT;
		if (pSession->shX > dfRANGE_MOVE_RIGHT)  pSession->shX = dfRANGE_MOVE_RIGHT;
		if (pSession->shY < dfRANGE_MOVE_TOP)    pSession->shY = dfRANGE_MOVE_TOP;
		if (pSession->shY > dfRANGE_MOVE_BOTTOM) pSession->shY = dfRANGE_MOVE_BOTTOM;
	}
}