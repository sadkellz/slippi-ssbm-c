#ifndef CUSTOM_VS_C_H
#define CUSTOM_VS_C_H

#include "../../../m-ex/MexTK/mex.h"
#include "../../3v1/CustomGame.h"
#include "../../../Common.h"
#include "../../../ExiSlippi.h"
#include "../../../Slippi.h"

typedef struct SharedMinorData {
    void* data;
} SharedMinorData;

void (*VS_Think)() = (void *) 0x8016D800;
void (*VS_Load)(MatchInit* minor_data) = (void *) 0x8016E934;
void (*VS_Exit)(ScDataMatchExit* minor_data) = (void *) 0x8016E9C8;
void (*Match_NoContestOrRetry)(int unk, int result_type) = (void *) 0x8016cf4c;

ScDataVS* (*VS_GetData)() = (ScDataVS *) 0x801a5244;

ScDataMatchExit *stc_last_match = (ScDataMatchExit *)0x80479d9c;
int *stc_frame_count = (int *)0x80479D60;
MajorScene *stc_mj_table = (MajorScene *)0x803daca4;


// 3v1 Functions
// void SortTeam

#endif