#ifndef SLIPPI_SSS_MAJOR_C
#define SLIPPI_SSS_MAJOR_C

#include "SSS.h"

void SlpSss_Prep(MinorScene *minor_data) {
    SSS_Enter(minor_data);
    return;
}

void SlpSss_Decide(MinorScene *minor_data) {
    // Get our exit data and check how the SSS was exited
    VSMinorData *exit_data = Scene_GetExitData(minor_data);

    if (exit_data->exit_kind == 0) {
        // return to the CSS
        Scene_SetNextMinor(SLP_SCENE_CSS); 
        return;
    }

    // set stage as selected
    R13_U8(R13_OFFSET_CHOSESTAGE) = 1;
    MatchStateResponseBuffer *msrb = MSRB();

    // advance to the splash screen if both players are ready
    if (msrb->isLocalPlayerReady && msrb->isRemotePlayerReady) {
        Scene_SetNextMinor(SLP_SCENE_SPLASH);
    }
    // return to the CSS if we have to wait
    else {
        Scene_SetNextMinor(SLP_SCENE_CSS);
    }

    return;
}


#endif
