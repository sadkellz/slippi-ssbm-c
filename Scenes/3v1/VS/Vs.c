#ifndef CUSTOM_VS_C
#define CUSTOM_VS_C

#include "Vs.h"


void minor_think() {
	// 3v1
	if (IsCustomMode()) {
		if (*stc_frame_count == 0) {
			OSReport("Set Stocks\n");

			// exit early if there is no solo player
			if (stc_tvo_characters->solo_player > 4) {
				// OSReport("Solo player is not valid: %d\n", stc_tvo_characters->solo_player);
				Match_EndImmediate();
				return;
			}

			for (int i = 0; i < 4; i++) {
				if (i != stc_tvo_characters->solo_player) {
					Fighter_SetStocks(i, 1);
				}
			}
		}
	}

	VS_Think();
	return;
}

void minor_load(MatchInit* minor_data) {
	VS_Load(minor_data);
	return;
}

void minor_exit(ScDataMatchExit* minor_data) {
	SlippiOnlineDataBuffer *odb = R13_PTR(R13_OFFSET_ODB_ADDR);

	// get our local player data
	u8 local_slot = odb->local_player_idx;
	CharacterKind ckind = stc_last_match->match_standings.ply_standings[local_slot].ckind;
	u8 stocks = stc_last_match->match_standings.ply_standings[local_slot].stock_num;
	
	OSReport("local_slot: %d\n", local_slot);
	OSReport("last match ckind: %d\n", ckind);
	OSReport("last match stocks: %d\n", stocks);

	// set our last played
	stc_tvo_characters->last_played = ckind;
	
	// move onto the next characer if we ran out of stocks
	if (stocks == 0) {
		TVO_SET_PLAYED(stc_tvo_characters, ckind);
	}

	VS_Exit(minor_data); // this will populate our standings

	bp();
	bool success = Tvo_WasMatchSuccessful(minor_data);
	OSReport("Success: %d\n", success);

	return;
}


#endif


