#ifndef SLIPPI_SPLASH_MAJOR_H
#define SLIPPI_SPLASH_MAJOR_H

#include "../../Major.h"

#pragma pack(1)
typedef struct SplashScreenData {
    int scale_type; // 0-3 - affects the characters model scale
    int match_type; // aka target/trophy/allstar etc...
    u8 nametag;
    u8 text_disp; // 0x78 = char name, 0 = nametag
    u8 progress; // roadmap progress
    u8 ply_num_left;
    u8 ply_num_right;
    u8 left_chars[3];
    u8 right_chars[3];
    u8 left_colors[3];
    u8 right_colors[3];
    u8 x19[6];
} SplashScreenData;

#pragma pack()
typedef struct SplashLocalPlayerData {
    u8 ckind;
    u8 costume;
    u8 difficulty;
    u8 stocks;
    u8 text_disp;
} SplashLocalPlayerData;

static SplashScreenData *stc_splash_data = (SplashScreenData*)0x80490880;
static void* (*Splash_GetStaticData)(void) = (void *) 0x8017eb30;
static void (*Splash_UnkPreload)(int arg0) = (void *) 0x80018c2c;
static void (*Splash_UnkPreload2)(int arg0) = (void *) 0x80017700;

void SlpSplash_Prep(MinorScene *minor_data);
void SlpSplash_Decide(MinorScene *minor_data);
void SlpSplash_ColorOverwrite(MatchInit *match_data);

#endif 
