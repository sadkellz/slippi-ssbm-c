#ifndef CUSTOM_VS_C_H
#define CUSTOM_VS_C_H

#include "../../../m-ex/MexTK/mex.h"
#include "../../3v1/CustomGame.h"
#include "../../../Common.h"
#include "../../../ExiSlippi.h"
#include "../../../Slippi.h"

Text *text;
// JOBJSet *level_hud;

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

// should eventually setup some sort of text system
// void SetupVsText() {
// 	GXColor color = {240, 240, 240, 255};
// 	GXColor green = {0, 255, 0, 255};

//    	Text_CreateCanvas(0, 0, 9, 13, 0, 11, 0, 3328);
// 	text = Text_CreateText(0, 0);
// 	text->kerning = 1;
// 	text->align = 0;
// 	text->trans = (Vec3){-22.f, 13.f, 0.f};
// 	text->scale = (Vec2){0.05f, 0.05f};
// 	text->color = color;
//     int level = 5;
// 	int header = Text_AddSubtext(text, 0.0f, 0.0f, "%d", level);
// }

// 3v1 Functions


#endif