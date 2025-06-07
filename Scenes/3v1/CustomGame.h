#ifndef SLIPPI_CUSTOM_H
#define SLIPPI_CUSTOM_H

#include "../../m-ex/MexTK/mex.h"
#include "../../Common.h"
#include <stdbool.h>

// constants
#define R13_OFFSET_ISCUSTOM -0x4DA7 // some single button mode offset, hopefully unused elsewhere
#define CSS_CORNER_XTHRESH -20.f
#define CSS_CORNER_YTHRESH 22.f
#define TVO_MAX_LEVEL 5
#define TVO_CHAR_COUNT 26
#define TVO_NOCHAR 0x20

extern Text *text;
    // level hud
#define LVLHUD_ROT -0.2f
#define LVLHUD_XOFST -3.5f
#define LVLHUD_YOFST 8.f
#define LVLHUD_ZOFST -1.f
#define LVLHUD_SCALE 0.5f
#define LVLHUD_ALPHA 0.75f

// macros
#define TVO_HAS_PLAYED(chardata, index) ((chardata)->has_played & (1 << (index)))
#define TVO_SET_PLAYED(chardata, index) ((chardata)->has_played |= (1 << (index)))
#define TVO_CLEAR_PLAYED(chardata, index) ((chardata)->has_played &= ~(1 << (index)))
#define CLAMP(val, min, max) ((val) < (min) ? (min) : (val) > (max) ? (max) : (val))

// testing
#define TVO_TESTING


// Structs
// typedef struct MexMajorScene {
//     u8 is_preload;
//     u8 major_id;
//     void *onLoad;
//     void *onExit;
//     void *onBoot;
//     void *MinorScene; // array of minor scenes
//     int x40;
// } MexMajorScene;

typedef struct TvoCharacterData {
    u32 has_played; 
    CharacterKind last_played;
    u8 solo_player;
    bool match_success;
    u8 player_levels[4];
    u8 char_remaining;
} TvoCharacterData; // 0x3F - 63 bytes max

typedef struct MatchResults {
    int x0;                           // 0x24c
    u8 end_kind;                      // 0x250
    u8 x5;                            // 0x251
    u8 is_teams;                      // 0x252
    int time_frames;                  // 0x254 how many frames passed in the match
    u8 xc;                            // 0x258
    u8 winner_num;                    // 0x259 is greater than 1 when a tie occurs
    u8 team_winner_num;
    u8 placings[4];                   // 0x25a - 0x25d, array of player indices in order of placement
    int x14;                          // 0x260
    int x18;                          // 0x264
    TeamStandings team_standings[5];  // 0x268
    PlayerStandings ply_standings[6]; // 0x2a4
} MatchResults;

typedef struct ScDataMatchExit {
    int x0;
    int x4;
    MatchResults match_standings;
    u8 unk[0x2278];
} ScDataMatchExit;

typedef struct MatchInfoRules {
    unsigned char matchType : 3;
    unsigned char hudPos : 3;
    unsigned char timer : 2;

    unsigned char timer_unk2 : 1;
    unsigned char unk4 : 1;
    unsigned char hideReady : 1;
    unsigned char hideGo : 1;
    unsigned char isDisableMusic : 1;
    unsigned char unk3 : 1;
    unsigned char timer_unk : 1;
    unsigned char unk2 : 1;

    unsigned char unk9 : 1;
    unsigned char disableOffscreenDamage : 1;
    unsigned char unk8 : 1;
    unsigned char isSingleButtonMode : 1;
    unsigned char isDisablePause : 1;
    unsigned char unk7 : 1;
    unsigned char isCreateHUD : 1;
    unsigned char unk5 : 1;

    unsigned char isShowScore : 1;        // 0x80
    unsigned char isShowAnalogStick : 1;  // 0x40
    unsigned char isCheckForZRetry : 1;   // 0x20
    unsigned char isShowZRetry : 1;       // 0x10
    unsigned char isCheckForLRAStart : 1; // 0x08
    unsigned char isShowLRAStart : 1;     // 0x04
    unsigned char isHidePauseHUD : 1;     // 0x02
    unsigned char timerRunOnPause : 1;    // 0x01
    unsigned char unk11 : 1;             // 0x80
    unsigned char isCheckStockSteal : 1; // 0x40
    unsigned char isRunStockLogic : 1;   // 0x20
    unsigned char unk1f : 5;             // 0x1f
    unsigned char no_check_end : 1;        // 0x80
    unsigned char isSkipUnkStockCheck : 1; // 0x40
    unsigned char no_hit : 1;              // 0x20
    unsigned char unk12 : 5;               // 0x01
    u8 bombRain; // 0xFF
    u8 match_end_gfx_type; // 0xFF
    bool is_teams; // 0xFF
    u8 use_ko_count; // 0xFF
    u8 unk_alt_music; // 0xFF
    s8 itemFreq; // 0xFF
    u8 sd_penalty; // 0xFF
    u8 unk16; // 0xFF
    u16 stage; // 0xFFFF
    int timerSeconds : 32; // 0xFFFFFFFF
    u8 timerSubSeconds; // 0xFF
    int unk17; // 0xFFFFFFFF
    unsigned long long itemSwitch : 64; // 0xFFFFFFFF FFFFFFFF
    int unk18; // 0xFFFFFFFF
    float quake_mult;
    float dmg_ratio;
    float match_speed;
    int x34;
    int x38;
    void *x3C_pause_cb;
    void *onStartMelee;
    void *onMatchFrame1;
    void *onMatchFrame2;
    void *onMatchEnd;
    bool x50_is_singleplayer;
    void *onCheckPause;
    int x58;
} MatchInfoRules;

typedef struct MatchInfoBlock {
    MatchInfoRules rules;
    PlayerData player_data[6];
} MatchInfoBlock;

// static melee pointers
// int *css = (int *)0x80479D60;
u8 *stc_match_result = (u8 *)0x8046b6a8; // accessing this from stc_match doesnt work? its 2 bytes off?
TvoCharacterData *stc_tvo_characters = (TvoCharacterData *)0x803eadc8; // this is some debug strings for camera screenshot // size 0x3F

void (*HUD_AddAnimsByCharIndex)(JOBJ *jobj, int anim_idx, void *joint_anim, void *mat_anim, void *shape_anim) = (void *) 0x8000c07c;
u32 (*CKindToId)(int id, int secondary) = (void *) 0x800325c8;
void (*TOBJ_Setup)(TOBJ *tobj) = (void *) 0x80360950;
void (*MOBJ_Setup)(MOBJ *mobj) = (void *) 0x80363a24;


// tvo functions

void Tvo_GetSoloPlayer(MatchInfoBlock *data) {
    u8 team_counts[4] = {0};
    u8 team_players[4] = {0};
    int unique_teams = 0;

    for (int i = 0; i < 4; i++) {
        u8 team = data->player_data[i].team;
        if (team_counts[team] == 0)
            unique_teams++;
        team_counts[team]++;
        team_players[team] = i;
    }

    if (unique_teams > 2) {
        stc_tvo_characters->solo_player = 255;
        return;
    }

    for (int t = 0; t < 4; t++) {
        if (team_counts[t] == 1) {
            stc_tvo_characters->solo_player = team_players[t];
            return;
        }
    }

    stc_tvo_characters->solo_player = 255;
}


bool Tvo_WasMatchSuccessful(ScDataMatchExit *data) {
    u8 result = *stc_match_result;

    if (result == 7 || result == 9) {
        return false;
    }

    OSReport("Match Result %x\n", *stc_match_result);
    return true;
}

void Tvo_DataInit() {
    if (strncmp(stc_tvo_characters, "cant", 4) == 0) {
        memset(stc_tvo_characters, 0, sizeof(TvoCharacterData));
        stc_tvo_characters->last_played = -1;
        OSReport("Init 3v1 Characters\n");
    } 
}

void Tvo_GetCharactersRemaining() {
    int char_count = 0;
    for (int i = 0; i < TVO_CHAR_COUNT; i++) {
        if (TVO_HAS_PLAYED(stc_tvo_characters, i))
        {
            char_count++;
        }
    }
    stc_tvo_characters->char_remaining = TVO_CHAR_COUNT - char_count;
}

void Tvo_GetCharacter(CharacterKind last_played, CharacterKind *kind_out) {

    // exit early if desync/quit out
    if (!stc_tvo_characters->match_success && (int)last_played != -1) {
        // check if last played is valid
        if (last_played > 0) {
            OSReport("Last Played: %d\n", last_played);
            *kind_out = last_played;
            return;
        }
        // if this is the first match, just resume?
    }

    // First character selection
    if (last_played == (CharacterKind)-1) {
        *kind_out = (CharacterKind)HSD_Randi(26);
        OSReport("First character selected: %d\n", *kind_out);
        return;
    }

    // If we've lost with this character before, pick a new one
    if (TVO_HAS_PLAYED(stc_tvo_characters, last_played)) {
        CharacterKind new_kind;
        do {
            new_kind = (CharacterKind)HSD_Randi(TVO_CHAR_COUNT);
            // Ensure we don't pick the same character we just lost with
        } while (new_kind == last_played && TVO_HAS_PLAYED(stc_tvo_characters, new_kind));
        *kind_out = new_kind;
        OSReport("Lost with char %d, switching to %d\n", last_played, *kind_out);
    }
    // Keep winning with current character
    else {
        *kind_out = last_played;
        OSReport("Keeping winning character: %d\n", *kind_out);
    }
}

void Tvo_GetStage(u8 *stage_out) {
    enum GrExternal stage_list[] = {
        GRKINDEXT_IZUMI, GRKINDEXT_PSTAD, GRKINDEXT_STORY,
        GRKINDEXT_BATTLE, GRKINDEXT_FD, GRKINDEXT_OLDPU 
    };

    const int stage_count = sizeof(stage_list) / sizeof(stage_list[0]);
    int random_idx = HSD_Randi(stage_count);
    *stage_out = stage_list[random_idx];
}

void Tvo_SetPlayerLevels() {
    for (int i = 0; i < 4; i++) {
        // dont run on solo player
        if (i != stc_tvo_characters->solo_player) {
            int stocks = Fighter_GetStocks(i);
            if (stocks <= 0) {
                if (stc_tvo_characters->player_levels[i] > 0)
                    stc_tvo_characters->player_levels[i]--;
            }
            else {
                if (stc_tvo_characters->player_levels[i] < TVO_MAX_LEVEL)
                    stc_tvo_characters->player_levels[i]++;
            }
        }
        
        OSReport("player %d level %d\n", i, stc_tvo_characters->player_levels[i]);
        
    }
}

void Tvo_LoadLevelHud() {
    JOBJSet **dmg_num;

    // load damage number set
    Archive_GetSections(*stc_ifall_archive, &dmg_num, "DmgNum_scene_models", 0);
    GOBJ *dmg_gobj = JOBJ_LoadSet(
        0, 
        *dmg_num, 
        0, 
        0.f, 
        14, 
        11, 
        0, 
        0);
    
    
    JOBJ *dmg_jobj = (JOBJ *)dmg_gobj->hsd_object;
    
    // add our animations
    HUD_AddAnimsByCharIndex(dmg_jobj, 0, (*dmg_num)->animjoint, (*dmg_num)->matanimjoint, (*dmg_num)->shapeaninjoint);

    // get our individual damage numbers so we can modify them
    JOBJ *numbers[4];
    JOBJ_GetChild(dmg_jobj, &numbers[0], 1, -1);
    JOBJ_GetChild(dmg_jobj, &numbers[1], 2, -1);
    JOBJ_GetChild(dmg_jobj, &numbers[2], 3, -1);
    JOBJ_GetChild(dmg_jobj, &numbers[3], 4, -1);

    // disable "%"
    JOBJ_SetFlags(numbers[3], JOBJ_HIDDEN);

    // animate the numbers to our levels
    for (int i = 0, j = 0; i < 4; i++) {
        if (i == stc_tvo_characters->solo_player)
            continue;

        u8 level_index = stc_tvo_characters->player_levels[i];
        
        if (level_index == 0) {
            JOBJ_SetFlagsAll(numbers[j], JOBJ_HIDDEN);
            j++;
            continue;
        }

        float frame = level_index * 2.0f;

        // animate
        JOBJ_ForEachAnim(numbers[j], 0x6, 0x400, AOBJ_ReqAnim, 1, frame);
        JOBJ_AnimAll(numbers[j]);
        JOBJ_ForEachAnim(numbers[j], 0x6, 0x400, AOBJ_StopAnim, 6, 0, 0);

        // set position, rotation, and alpha
        Vec3 *pos;
        pos = Match_GetPlayerHUDPos(i);
        numbers[j]->trans = (Vec3){ pos->X + LVLHUD_XOFST, pos->Y + LVLHUD_YOFST, pos->Z};
        numbers[j]->scale = (Vec3){LVLHUD_SCALE, LVLHUD_SCALE, LVLHUD_SCALE};
        numbers[j]->rot.Z = LVLHUD_ROT;
        numbers[j]->dobj->mobj->mat->alpha = 0.75f;

        // increment joint num
        j++;
    }

}

void Tvo_Player_ApplyHitstunMod(GOBJ *fighter) {
    float level_scale[6] = {1.f, 1.15f, 1.35f, 1.75f, 2.5f, 3.f};
    FighterData *ft_data = (FighterData*)fighter->userdata;
    int slot = ft_data->ply;
    int level = stc_tvo_characters->player_levels[slot];

    // check if we are the solo player
    if (stc_tvo_characters->solo_player == slot)
        return;

    // apply hitstun multiplier
    ft_data->dmg.behavior = level_scale[level];
    return;
}

static inline void SetupIcon(JOBJ *icon_jobj, GXColor color) {
    DOBJ *bg_dobj   = icon_jobj->dobj;
    DOBJ *char_dobj = icon_jobj->dobj->next;

    // reorder so bg is drawn after char
    bg_dobj->next   = 0x0;
    icon_jobj->dobj = char_dobj;
    char_dobj->next = bg_dobj;

    bg_dobj->mobj->mat->diffuse = color;
    bg_dobj->mobj->mat->ambient = color;
    bg_dobj->mobj->mat->alpha   = 0.5f;

    bg_dobj->mobj->tobj->blending   = 0.f;
    bg_dobj->mobj->tobj->scale      = (Vec3){1.f, 60.f, 1.f};
    bg_dobj->mobj->tobj->magFilt    = 0;
    bg_dobj->mobj->tobj->translate  = (Vec3){0.f, 8.5f, 0.f};

    JOBJ_ForEachAnim(icon_jobj, 0x6, 0x400, AOBJ_ReqAnim, 1, 13.f);
    JOBJ_AnimAll(icon_jobj);
    JOBJ_ForEachAnim(icon_jobj, 0x6, 0x400, AOBJ_StopAnim, 6, 0, 0);
    JOBJ_RemoveAnimAll(icon_jobj);
}

void Tvo_Css_SetIcons() {
    JOBJ *menu_jobj = (JOBJ*)(*stc_css_menugobj)->hsd_object;
    GXColor RED    = (GXColor){255, 0, 0, 255};
    GXColor ORANGE = (GXColor){255, 128, 0, 255};

    // zelda/sheik logic
    bool played_zelda = TVO_HAS_PLAYED(stc_tvo_characters, 0x12);
    bool played_sheik = TVO_HAS_PLAYED(stc_tvo_characters, 0x13);

    // iterate through the css icons, skipping zelda
    for (int i = 0; i < 25; i++) {
        if (i == 15) continue;

        MnSlChrIcon icon = stc_css_data->icons[i];
        if (!TVO_HAS_PLAYED(stc_tvo_characters, icon.c_kind)) continue;

        JOBJ *icon_jobj;
        JOBJ_GetChild(menu_jobj, &icon_jobj, icon.joint_id_1p, -1);
        SetupIcon(icon_jobj, RED);
    }

    // Zelda and Sheik
    if (played_zelda || played_sheik) {
        JOBJ *icon_jobj;
        MnSlChrIcon icon = stc_css_data->icons[15];
        JOBJ_GetChild(menu_jobj, &icon_jobj, icon.joint_id_1p, -1);
        GXColor color;

        if (played_zelda && played_sheik) {
            color = RED;
        }
        else if (played_zelda || played_sheik) {
            color = ORANGE;
        }
        
        SetupIcon(icon_jobj, color);
    }
}


#endif
