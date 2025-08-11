#ifndef SLIPPI_MAJOR_H
#define SLIPPI_MAJOR_H

#include "../../m-ex/MexTK/mex.h"
#include "../../Common.h"
#include "../../Slippi.h"
#include "../../ExiSlippi.h"

#define SLP_MAJOR_ID 8
#define KB_HASHTAG (u32)0x81940000
#define GAME_PREP_MAX_RESULT_COUNT 9
#define LGL_LIMIT 45

typedef enum SlippiMinorScene {
    SLP_SCENE_CSS = 0,
    SLP_SCENE_SSS = 1,
    SLP_SCENE_VS = 2,
    SLP_SCENE_RESULTS = 3,
    SLP_SCENE_SPLASH = 4,
    SLP_SCENE_RANKED = 5
} SlippiMinorScene;

#pragma pack(1)
typedef struct GamePrepStaticBlock
{
    u8 max_games;                    // 0x00 GPDO_MAX_GAMES
    u16 cur_game;                     // 0x01 GPDO_CUR_GAME
    u8 score_by_player[2];           // 0x03 GPDO_SCORE_BY_PLAYER
    u8 prev_winner;                  // 0x05 GPDO_PREV_WINNER
    u8 tiebreak_game_num;            // 0x06 GPDO_TIEBREAK_GAME_NUM
    u8 game_results[GAME_PREP_MAX_RESULT_COUNT]; // 0x07 GPDO_GAME_RESULTS
    u16 last_stage_win_by_player[2];  // 0x10 GPDO_LAST_STAGE_WIN_BY_PLAYER
    u8 color_ban_active;             // 0x14 GPDO_COLOR_BAN_ACTIVE
    u8 color_ban_char;               // 0x15 GPDO_COLOR_BAN_CHAR
    u8 color_ban_color;              // 0x16 GPDO_COLOR_BAN_COLOR
    u8 last_game_end_mode;           // 0x17 GPDO_LAST_GAME_END_MODE
    u32 fn_compute_ranked_winner;     // 0x18 GPDO_FN_COMPUTE_RANKED_WINNER
} GamePrepStaticBlock;

u8 SinglesDetermineWinner(u8 winner_idx);

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

static u32 **keyboard_data = (u32**)0x803EDC1C;
static u8 *slp_local_port = (u8*)0x8045ABF6; // correct name?
static ScDataVS *stc_mode_data = (ScDataVS*)0x8045AC50; // array has a size of 12?
static ScDataRst *stc_last_match = (ScDataRst*)0x80479d98;

static void *(*Scene_GetEnterData)(void *minor_data) = (void *) 0x801a427c;
static void *(*Scene_GetExitData)(void *minor_data) = (void *) 0x801a4284;
static void (*CSS_InitDataSingleplayer)(void *css_data, u8 match_type, CharacterKind ckind, u8 stocks, u8 color, u8 nametag, u8 cpu_lvl, u8 slot) = (void *) 0x801b06b0;
static void (*Event_CSSDecide)(void *minor_data) = (void *) 0x801baad0;
static void (*SSS_Enter)(void *minor_data) = (void *) 0x801b1514;
static void (*VS_Enter)(void *minor_data) = (void *) 0x801b1588;
static void (*VS_Exit)(void *minor_data) = (void *) 0x801b15c8;
static int (*PlayerBlock_GetLedgeGrabs)(int slot) = (int *) 0x80040af0;
static u16 (*PlayerBlock_GetPercent)(int slot) = (u16 *) 0x800342b4;

u8 GetTeamCount(u8 team_idx, PlayerData *ply_data);

#endif
