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

        // check if we've clicked the top left corner
        CheckForModeSwitch();
        
        if (IsCustomMode()) {
            SlippiCSSDataTable *css_data;
            CharacterKind c_kind;
            CharacterKind last_played = stc_tvo_characters->last_played;
            u8 stage_id;
            u8 team;

            // this lets us skip the stage select, also has to be set before we've locked in
            R13_U8(R13_OFFSET_ISWINNER) = 1;
            R13_U8(R13_OFFSET_CHOSESTAGE) = 1;
            
            css_data = GetSlpCSSDT();

            // init our 3v1 character data
            Tvo_DataInit();

            // we run most of the setup after we're locked in
            if (css_data->prevLockInState) {
                stage_id = 0x1F; // battlefield default
                bool stage_option = false;
                
                // get stage
                if (css_data->msrb->localName == css_data->msrb->p1Name) {
                    Tvo_GetStage(&stage_id);
                    stage_option = true;
                }
                
                // getting character
                Tvo_GetCharacter(last_played, &c_kind);

                // team init
                team = css_data->msrb->gameInfoBlock.playerData[css_data->msrb->localPlayerIndex].team;
                Tvo_GetSoloPlayer(css_data->msrb->gameInfoBlock);
                if (stc_tvo_characters->solo_player > 4) {
                    OSReport("No solo player\n");
                }

                // override match selections
                SetMatchSelections(c_kind, team, 1, stage_id, stage_option, team);
                return;
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

    // debug match selections
    for (int i = 0; i < 26; i++) {
        OSReport("has played %d: %d\n", i, (stc_tvo_characters->has_played & (1 << i)) ? 1 : 0);
    }
    // OSReport("CSS_exit\n");
}

#endif SLIPPI_CSS_SETUP_C