#ifndef SLIPPI_VS_MAJOR_C
#define SLIPPI_VS_MAJOR_C

#include "VS.h"


void SlpVs_Prep(MinorScene *minor_data) {
    VS_Enter(minor_data);
    return;
}

void SlpVs_Decide(MinorScene *minor_data) {
    VS_Exit(minor_data);

    ExiSlippi_MatchState_Response *msrb = ExiSlippi_LoadMatchState(0);
    u8 online_mode = R13_U8(R13_OFFSET_ONLINE_MODE);
    u8 local_player = msrb->local_player_idx;

    // handle ranked
    if (online_mode == ONLINE_MODE_RANKED) {
        
    }

    // set winner logic
    MatchStandings standings = stc_last_match->match_standings;
    u8 is_winner = 1;
    // check if we were in the same mode
    if (stc_match->x2450 != (int)standings.is_teams) {
        is_winner = 0;
    }
    // did we play last match?
    if (standings.ply_standings[local_player].pkind == 3) {
        is_winner = 0;
    }
    // quit out?
    if (standings.end_kind == 7) {
        // team logic
        if (standings.is_teams) {
            
        }
    }

    HSD_Free(msrb);
    Scene_SetNextMinor(SLP_SCENE_CSS);
    return;
}



#endif
