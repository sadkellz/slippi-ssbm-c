#ifndef SLIPPI_CSS_SETUP_C
#define SLIPPI_CSS_SETUP_C

#include "Main.c"

void (*CSS_think)() = (void *) 0x802669F4;
void (*CSS_load)() = (void *) 0x8026688C;
void (*CSS_exit)() = (void *) 0x80266D70;
void (*CSS_LoadButtonInputs)() = (void *) 0x80263258;

static bool is_teams;
Text *text;

//Runs every frame during CSS
void minor_think() {
    CSS_think();
    UpdateOnlineCSS();

    // 3v1
    if (is_teams) {
        CheckForModeSwitch();
        
        if (IsCustomMode()) {
            SlippiCSSDataTable *css_data;

            // this lets us skip the stage select
            R13_U8(R13_OFFSET_ISWINNER) = 1;
            R13_U8(R13_OFFSET_CHOSESTAGE) = 1;

            // init our 3v1 character data
            if ((u8)stc_tvo_characters->has_played[0] == 99) {
                stc_tvo_characters = HSD_MemAlloc(26);
                memset(stc_tvo_characters, 0, 26);
            }

            // We overwrite selections to make sure remote player can't cheat their selections
            css_data = GetSlpCSSDT();
            
            if (css_data->prevLockInState) {
                u8 stage_id = 0x1F;
                bool stage_option = false;
                if (css_data->msrb->localPlayerIndex == 0) {
                    stage_id = 20;
                    stage_option = true;
                }
                SetMatchSelections(24, 0, 1, stage_id, stage_option, 0);
            }
        }
        return;
    }
    return;
}

//Runs when CSS is loaded
void minor_load(VSMinorData *minor_data) {
    CSS_load();
    InitOnlineCSS();

    // check if teams
    if (R13_U8(R13_OFFSET_ONLINE_MODE) == 3) {
        is_teams = true;
    }
    else {
        is_teams = false;
    }

    // OSReport("CSS_load\n");
    return;
}

//Runs when leaving CSS
void minor_exit(VSMinorData *minor_data) {
    CSS_exit();
    // OSReport("CSS_exit\n");
}

#endif SLIPPI_CSS_SETUP_C