#ifndef SLIPPI_CSS_SETUP_C
#define SLIPPI_CSS_SETUP_C

#include "Main.c"

void (*CSS_think)() = (void *) 0x802669F4;
void (*CSS_load)() = (void *) 0x8026688C;
void (*CSS_exit)() = (void *) 0x80266D70;
void (*CSS_LoadButtonInputs)() = (void *) 0x80263258;

static inline void SetMatchSelections(u8 char_id, u8 char_color, u8 char_option, u16 stage_id, u8 stage_option, u8 team_id) {
    ExiSlippi_SetSelections_Query *ssq = calloc(sizeof(ExiSlippi_SetSelections_Query));
    ssq->command = ExiSlippi_Command_SET_MATCH_SELECTIONS;
    ssq->team_id = team_id;
    ssq->char_id = char_id;
    ssq->char_color_id = char_color;
    ssq->char_option = char_option;
    ssq->stage_id = stage_id;
    ssq->stage_option = stage_option;
    ssq->online_mode = -1;  // Ranked. Doesn't actually do anything in Dolphin atm
    ExiSlippi_Transfer(ssq, sizeof(ExiSlippi_SetSelections_Query), ExiSlippi_TransferMode_WRITE);
    HSD_Free(ssq);
}


//Runs every frame during CSS
void minor_think() {
    SlippiCSSDataTable *css_data;

    CSS_think();
    UpdateOnlineCSS();
  
    // We overwrite selections to make sure remote player can't cheat their selections
    css_data = GetSlpCSSDT();

    if (css_data->prevLockInState) {
        // R13_U8(R13_OFFSET_CHOSESTAGE) = 1;
        // R13_U8(R13_OFFSET_ISWINNER) = 1;
        // osq->stage_id = 20;
        // for (int i = 0; i < 4; i++) {
        //     osq->chars[i].char_color_id = 2;
        //     osq->chars[i].char_id = 24;
        //     osq->chars[i].is_set = 1;

        // }

        // osq->command = ExiSlippi_Command_OVERWRITE_SELECTIONS;
        // ExiSlippi_Transfer(osq, sizeof(ExiSlippi_OverwriteSelections_Query), ExiSlippi_TransferMode_WRITE);
        // OSReport("Overwritten selections\n");

        SetMatchSelections(24, 0, 1, 20, 1);
    }
}

//Runs when CSS is loaded
void minor_load(VSMinorData *minor_data) {
    CSS_load();
    InitOnlineCSS();

    R13_U8(R13_OFFSET_ISWINNER) = 1;
    R13_U8(R13_OFFSET_CHOSESTAGE) = 1;
    *stc_css_is_ready_timer = 0;
    OSReport("CSS_load\n");
}

//Runs when leaving CSS
void minor_exit(VSMinorData *minor_data) {
    // ExiSlippi_MatchState_Response *msrb;
    // SetMatchSelections(25, 0, 0, 20, -2);
    CSS_exit();
    // msrb = ExiSlippi_LoadMatchState(0);
    // OSReport("msrb->game_info_block: %08X\n", msrb->game_info_block);
    OSReport("CSS_exit\n");
}

#endif SLIPPI_CSS_SETUP_C