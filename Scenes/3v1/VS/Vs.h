#ifndef CUSTOM_VS_C_H
#define CUSTOM_VS_C_H

#include "../../../m-ex/MexTK/mex.h"

typedef struct SharedMinorData {
    void* data;
} SharedMinorData;

typedef struct MatchResults
{
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

typedef struct ScDataVictory
{
    int x0;
    int x4;
    MatchResults match_standings;
    u8 unk[0x2278];
} ScDataVictory;

void (*VS_Think)() = (void *) 0x8016D800;
void (*VS_Load)(MatchInit* minor_data) = (void *) 0x8016E934;
void (*VS_Exit)(ScDataVictory* minor_data) = (void *) 0x8016E9C8;

ScDataVS* (*VS_GetData)() = (void *) 0x801a5244;

void *stc_last_match = (void *)0x80479d98;
int *stc_frame_count = (int *)0x80479D60;
MajorScene *stc_mj_table = (MajorScene *)0x803daca4;


// 3v1 Functions
// void SortTeam

#endif