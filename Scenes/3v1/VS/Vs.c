#ifndef CUSTOM_VS_C
#define CUSTOM_VS_C

#include "Vs.h"
#include "../../../Core/Notifications/Notifications.c"
#include "../../CSS/Chat/Chat.c"

void minor_think() {
	// 3v1
	if (*stc_frame_count == 0) {
		// exit early if there is no solo player
		if (stc_tvo_characters->solo_player == 255) {
			Match_EndImmediate();
			return;
		}
	}

	VS_Think();
	return;
}

void minor_load(MatchInit* minor_data) {
	// original code
	VS_Load(minor_data);

	Tvo_LoadLevelHud();
	// create our player procs
	for (int i = 0; i < 4; i++) {
		GOBJ *fighter = Fighter_GetGObj(i);
		GObj_AddProc(fighter, Tvo_Player_ApplyHitstunMod, 13);
	}

	return;
}

void minor_exit(ScDataMatchExit* minor_data) {
	SlippiOnlineDataBuffer *odb = R13_PTR(R13_OFFSET_ODB_ADDR);

	stc_tvo_characters->match_success = Tvo_WasMatchSuccessful(minor_data);

	// get our local player data
	u8 local_slot = odb->local_player_idx;
	u8 ckind = Fighter_GetExternalID(local_slot);
	u8 stocks = Fighter_GetStocks(local_slot);
	// u8 stocks = stc_last_match->match_standings.ply_standings[local_slot].stock_num;

	// set our last played
	stc_tvo_characters->last_played = ckind;
	OSReport("last played: %d\n", ckind);
	OSReport("stocks: %d\n", stocks);

	if(stc_tvo_characters->match_success) {
		// move onto the next characer if we ran out of stocks
		if (stocks == 0) {
			TVO_SET_PLAYED(stc_tvo_characters, ckind);
		}

		// set our levels
		Tvo_SetPlayerLevels();
	}

	// original code
	VS_Exit(minor_data);
	return;
}


#endif


