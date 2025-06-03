#ifndef SLIPPI_CUSTOM_H
#define SLIPPI_CUSTOM_H

#include "../../m-ex/MexTK/mex.h"
#include <stdbool.h>

#define R13_OFFSET_ISCUSTOM -0x4DA7 // some single button mode offset, hopefully unused elsewhere
#define CSS_CORNER_XTHRESH -20.f
#define CSS_CORNER_YTHRESH 22.f

extern Text *text;

typedef struct MexMajorScene
{
    u8 is_preload;
    u8 major_id;
    void *onLoad;
    void *onExit;
    void *onBoot;
    void *MinorScene; // array of minor scenes
    int x40;
} MexMajorScene;

bool IsCustomMode() {
    return R13_U8(R13_OFFSET_ISCUSTOM);
}

// int *css = (int *)0x80479D60;

#endif
