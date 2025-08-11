#ifndef SLIPPI_CSS_MAJOR_C
#define SLIPPI_CSS_MAJOR_C

#include "CSS.h"
#include "../../../../ExiSlippi.h"

void SlpCss_Prep(MinorScene *minor_data) {
    // Initialize Singleplayer CSS
    void *css_data = Scene_GetEnterData(minor_data);
    CSSBackup backup = stc_memcard->EventBackup;

    CSS_InitDataSingleplayer(css_data, 14, backup.c_kind, 0, backup.costume, backup.nametag, 0, backup.port);
    Preload_Invalidate();
    return;
}

void SlpCss_Decide(MinorScene *minor_data) {
    // Run event mode CSS SceneDecide to save HMN character choice
    Event_CSSDecide(minor_data);

    // Get our exit data and check how the CSS was exited
    VSMinorData *exit_data = Scene_GetExitData(minor_data);

    if (exit_data->exit_kind == CSSEXIT_MAINMENU) {
        return;
    }
    
    // Were advancing, so check for the direct modes
    u8 online_mode = R13_U8(R13_OFFSET_ONLINE_MODE);
    switch (online_mode) {
        case ONLINE_MODE_RANKED:
            break;
        case ONLINE_MODE_UNRANKED:
            Scene_SetNextMinor(SLP_SCENE_SPLASH);
            break;
        case ONLINE_MODE_DIRECT:
        case ONLINE_MODE_TEAMS:
            SlpCss_InitDirect();
            break;
    }

    // init splash
    // ScDataVS *sss_data = &stc_mode_data[0];
    // ExiSlippi_MatchState_Response *msrb = ExiSlippi_LoadMatchState(0);
    
    // MatchInit *match_block = (MatchInit*)msrb->game_info_block;
    // memcpy(&sss_data->match_init, match_block, 0x138);
    // stc_mode_data[0].match_init = *match_block;


    // HSD_Free(msrb);
    OSReport("Leaving CSS\n");
    return;
}

void SlpCss_InitRanked() {

}

void SlpCss_InitDirect() {
    u8 is_winner = R13_U8(R13_OFFSET_ISWINNER);

    // First match is random
    if (is_winner == 255) {
        Scene_SetNextMinor(SLP_SCENE_SPLASH);
        return;
    }
    // Winner doesnt pick stage
    else if (is_winner == 1) {
        Scene_SetNextMinor(SLP_SCENE_SPLASH);
        return;
    }
    // Loser decides
    else if (is_winner == 0) {
        u8 choose_stage = R13_U8(R13_OFFSET_CHOSESTAGE);
        if (choose_stage == 0) {
            Scene_SetNextMinor(SLP_SCENE_SSS);
            return;
        }
    }
    else {
        assert("Invalid is_winner value."); // shouldnt be possible, crash
    }
}






#endif SLIPPI_CSS_MAJOR_C
