#ifndef SLIPPI_MAJOR_C
#define SLIPPI_MAJOR_C

#include "Major.h"
#include "MinorSetup/CSS/CSS.c"
#include "MinorSetup/SSS/SSS.c"
#include "MinorSetup/VS/VS.c"
#include "MinorSetup/Splash/Splash.c"

// This mimics most of the logic in `Slippi Online Scene/main.asm`
// static GamePrepStaticBlock gameprep_block = {
//     .max_games = 0,
//     .cur_game = 0,
//     .score_by_player = {0, 0},
//     .prev_winner = 0,
//     .tiebreak_game_num = 0,
//     .game_results = {0, 0, 0, 0, 0, 0, 0, 0, 0},
//     .last_stage_win_by_player = {0, 0},
//     .color_ban_active = 0,
//     .color_ban_char = 0,
//     .color_ban_color = 0,
//     .last_game_end_mode = 0,
//     .fn_compute_ranked_winner = SinglesDetermineWinner,
// };
static GamePrepStaticBlock gameprep_block = {0};

// Initialize Minor Scene Array
static MinorScene __attribute__((__used__)) minor_scene[] = {
{
        .minor_id = SLP_SCENE_CSS,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = SlpCss_Prep,
        .minor_decide = SlpCss_Decide,
        .minor_kind = 8,
        .load_data = (void*)0x80497758,
        .unload_data = (void*)0x80497758,
    },
{
        .minor_id = SLP_SCENE_SSS,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = SlpSss_Prep,
        .minor_decide = SlpSss_Decide,
        .minor_kind = 9,
        .load_data = (void*)0x80480668,
        .unload_data = (void*)0x80480668,
    },
{
        .minor_id = SLP_SCENE_VS,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = SlpVs_Prep,
        .minor_decide = SlpVs_Decide,
        .minor_kind = 2,
        .load_data = (void*)0x80480530,
        .unload_data = (ScDataRst*)0x80479d98, // stc_last_match
    },
{
        .minor_id = SLP_SCENE_RESULTS,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = 0,
        .minor_decide = 0,
        .minor_kind = 5,
        .load_data = (void*)0x0,
        .unload_data = (void*)0x0,
    },
{
        .minor_id = SLP_SCENE_SPLASH,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = SlpSplash_Prep,
        .minor_decide = SlpSplash_Decide,
        .minor_kind = 32,
        .load_data = (void*)0x80490880,
        .unload_data = (void*)0x804d68d0,
    },
{
        .minor_id = SLP_SCENE_RANKED,
        .heap_kind = HEAPKIND_UNK3,
        .minor_prep = 0,
        .minor_decide = 0,
        .minor_kind = 80,
        .load_data = &gameprep_block,
        .unload_data = &gameprep_block,
    },
{
        .minor_id = -1,
        .heap_kind = 0,
        .minor_prep = 0,
        .minor_decide = 0,
        .minor_kind = -1,
        .load_data = 0,
        .unload_data = 0,
    }
};

void major_load(){
	OSReport("Slippi.major_load!\n");

    // Initialize hashtag letter
    (*keyboard_data[2]) = KB_HASHTAG;

    // Initialize some variables
    R13_U8(R13_OFFSET_NAME_ENTRY_MODE) = 0;
    R13_U8(R13_OFFSET_ISPAUSE) = 0;
    R13_U8(R13_OFFSET_USE_PREMADE_TEXT) = 0;
    R13_U8(R13_OFFSET_ISWIDESCREEN) = 0;

    // Set 1p port for CSS
    *slp_local_port = R13_U8(R13_OFFSET_LOCALPORT);

    // setup ranked callback to determine winner at the end of the match
    gameprep_block.fn_compute_ranked_winner = SinglesDetermineWinner;

    // set gpd data in minor
    minor_scene[SLP_SCENE_RANKED].load_data = &gameprep_block;
    minor_scene[SLP_SCENE_RANKED].unload_data = &gameprep_block;

    // Set up Zelda to select Sheik as default
    // MnSlChrIcon *icon_data = Slippi_GetCSSIconData();
    MnSlChrIcon *icon_data = stc_css_data->icons;
    int icon_num = Slippi_GetCSSIconNum();

    for (int i = 0; i < icon_num; i++){
        if (icon_data[i].c_kind == CKIND_ZELDA) {
            icon_data[i].c_kind = CKIND_SHEIK;
            break;
        }
    }

    return;
}

void major_exit(){
	OSReport("Slippi.major_exit!\n");

    // Restore Sheik back to Zelda
    MnSlChrIcon *icon_data = stc_css_data->icons;
    int icon_num = Slippi_GetCSSIconNum();

    for (int i = 0; i < icon_num; i++){
        if (icon_data[i].c_kind == CKIND_SHEIK) {
            icon_data[i].c_kind = CKIND_ZELDA;
            break;
        }
    }

    return;
}

u8 SinglesDetermineWinner(u8 winner_idx) {
    u8 end_kind = stc_last_match->match_standings.end_kind;
    // handle timeout
    if (end_kind == MATCHENDKIND_VSTIME) {
        int ply_one_lgrabs = PlayerBlock_GetLedgeGrabs(0);
        int ply_two_lgrabs = PlayerBlock_GetLedgeGrabs(1);

        // lgl check
        bool both_over = ply_one_lgrabs > LGL_LIMIT && ply_two_lgrabs > LGL_LIMIT;
        if (!both_over) {
            if (ply_one_lgrabs > LGL_LIMIT) {
                winner_idx = 1;
                return winner_idx;
            }
            else if (ply_two_lgrabs > LGL_LIMIT) {
                winner_idx = 0;
                return winner_idx;
            }
            // No one was over
        }
        // stock check
        int ply_one_stocks = Fighter_GetStocks(0);
        int ply_two_stocks = Fighter_GetStocks(1);
        if (ply_one_stocks > ply_two_stocks) {
            winner_idx = 0;
            return winner_idx;
        }
        else if (ply_two_stocks > ply_one_stocks) {
            winner_idx = 1;
            return winner_idx;
        }
        // same stocks, check damage
        int ply_one_damage = PlayerBlock_GetPercent(0);
        int ply_two_damage = PlayerBlock_GetPercent(1);
        if (ply_one_damage < ply_two_damage) {
            winner_idx = 0;
            return winner_idx;
        }
        else if (ply_two_damage < ply_one_damage) {
            winner_idx = 1;
            return winner_idx;
        }
        // same damage, tie
        return -1;
    }
    else if (end_kind == MATCHENDKIND_VSGAME) {
        // only check stocks
        int ply_one_stocks = Fighter_GetStocks(0);
        int ply_two_stocks = Fighter_GetStocks(1);
        if (ply_one_stocks > ply_two_stocks) {
            winner_idx = 0;
            return winner_idx;
        }
        else if (ply_two_stocks > ply_one_stocks) {
            winner_idx = 1;
            return winner_idx;
        }
        // same frame double ko
    }
    return -1;
}

u8 GetTeamCount(u8 team_idx, PlayerData *ply_data) {
    u8 count;

    for (int i = 0; i < 4; i++) {
        if (ply_data[i].team == team_idx) { count++; }
    }
    return count;
}

#endif
