#ifndef SLIPPI_CSS_SETUP_C
#define SLIPPI_CSS_SETUP_C

#include "Main.c"

void (*CSS_think)() = (void *) 0x802669F4;
void (*CSS_load)() = (void *) 0x8026688C;
void (*CSS_exit)() = (void *) 0x80266D70;
void (*CSS_LoadButtonInputs)() = (void *) 0x80263258;

static u32 default_rules[] = { 
    0x3201864c, 0xc3000000, 0x000000ff, 0xff6e001f,
    0x000001e0, 0x00000000, 0x00000000, 0x00000000,
    0xffffffff, 0xffffffff, 0x00000000, 0x3f800000,
    0x3f800000, 0x3f800000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000,
};

static bool is_init = false;
Text *text;
u16 stage;

//Runs every frame during CSS
void minor_think() {
    CSS_think();
    UpdateOnlineCSS();

    // 3v1
    if (!is_init) {
        SlippiCSSDataTable *css_data;
        CharacterKind c_kind;
        CharacterKind last_played = stc_tvo_characters->last_played;
        u8 team;
        u8 color;

        css_data = GetSlpCSSDT();
        CheckToDrawText();

        // this lets us skip the stage select, also has to be set before we've locked in
        R13_U8(R13_OFFSET_ISWINNER) = 1;
        R13_U8(R13_OFFSET_CHOSESTAGE) = 1;

        // we run most of the setup after we're locked in
        if (css_data->prevLockInState) {
            stage = 0x1F; // battlefield default
            
            // get stage
            Tvo_GetStage(&stage);
            // OSReport("stage_id: %d\n", stage);
            

            // getting character
            if (stc_tvo_characters->char_remaining != 0) {
                Tvo_GetCharacter(last_played, &c_kind);
            }
            else {
                c_kind = TVO_NOCHAR;
            }

            // override match selections
            team = css_data->teamIndex - 1;
            color = css_data->teamCostumeIndex - 1;
            
            OSReport("team: %d, color: %d\n", team, color);

            SetMatchSelections(c_kind, team, 1, stage, 1, team);
            is_init = true;
            return;
        }
    }
}

//Runs when CSS is loaded
void minor_load(VSMinorData *minor_data) {
    CSS_load();
    InitOnlineCSS();

    // init our 3v1 character data
    Tvo_DataInit();
    Tvo_GetCharactersRemaining();
    Tvo_Css_SetIcons();

    // OSReport("CSS_load\n");
    return;
}

//Runs when leaving CSS
void minor_exit(VSMinorData *minor_data) {
    SlippiOnlineDataBuffer *odb = R13_PTR(R13_OFFSET_ODB_ADDR);
    u8 local_slot = odb->local_player_idx;

    // debug match selections
    // for (int i = 0; i < 26; i++) {
    //     OSReport("has played %d: %d\n", i, (stc_tvo_characters->has_played & (1 << i)) ? 1 : 0);
    // }

    ExiSlippi_MatchState_Response *msrb = calloc(sizeof(ExiSlippi_MatchState_Response));
    ExiSlippi_LoadMatchState(msrb);
    MatchInfoBlock* match_block = (MatchInfoBlock*)msrb->game_info_block;
    
    // default rules
    memcpy(&match_block->rules, &default_rules, 0x60);

    // modify the ruleset for 3v1
    match_block->rules.is_teams = true;
    match_block->rules.stage = stage;

    // each person has to set the character in-case they arent "host"
    for (int i = 0; i < 4; i++) {
        if (match_block->player_data[i].c_kind == TVO_NOCHAR) {
            match_block->player_data[i].p_kind = 3;
        }
    }

    // stock logic
    // currently its still up to the players to set their correct team
    Tvo_GetSoloPlayer(match_block);
    if (stc_tvo_characters->solo_player == 255) {
        OSReport("No Solo Team\n");
        for (int i = 0; i < 4; i++) {
                match_block->player_data[i].stocks = 4;
            }
    }
    else {
        for (int i = 0; i < 4; i++) {
            if (stc_tvo_characters->solo_player != i) {
                match_block->player_data[i].stocks = 1;
            }
        }
    }

    // transfer our custom match settings
    ExiSlippi_SetMatchSettings *cms = calloc(sizeof(ExiSlippi_SetMatchSettings));
    cms->command = ExiSlippi_Command_SET_MATCH_SETTINGS;
    memcpy(cms->game_info_block, match_block, 312);
    ExiSlippi_Transfer(cms, 312, ExiSlippi_TransferMode_WRITE);


    OSReport("CSS_exit\n");
    CSS_exit();
    return;
}

#endif SLIPPI_CSS_SETUP_C