#ifndef SLIPPI_SPLASH_MAJOR_C
#define SLIPPI_SPLASH_MAJOR_C

#include "Splash.h"

static SplashScreenData splash_data = {
    .scale_type = 0,
    .match_type = 0,
    .nametag = 1,
    .text_disp = 0x78,
    .progress = 1,
    .ply_num_left = 0,
    .ply_num_right = 0,
    .left_chars = {33, 33, 33},
    .right_chars = {33, 33, 33},
    .right_colors = {0, 0, 0},
    .left_colors = {0, 0, 0},
};

void SlpSplash_Prep(MinorScene *minor_data) {
    OSReport("Splash Prep\n");
    ScDataVS *sss_data = &stc_mode_data[0];
    ExiSlippi_MatchState_Response *msrb = ExiSlippi_LoadMatchState(0);
    u8 online_mode = R13_U8(R13_OFFSET_ONLINE_MODE);
    MatchInit *match_block = (MatchInit*)msrb->game_info_block;
    memcpy(&sss_data->match_init, match_block, 0x138);

    // bp();
    // setup splash screen
    if (online_mode == ONLINE_MODE_TEAMS) {
        SlpSplash_ColorOverwrite(&sss_data->match_init);
        splash_data.scale_type = 1; // teams, will make the announcer say teams?

        // setup teams
        u8 local_team = sss_data->match_init.playerData[msrb->local_player_idx].team;
        OSReport("local team: %d\n", local_team);
        for (int i = 0; i < 4; i++) {
            PlayerData current_player = sss_data->match_init.playerData[i];

            // if this player is on our team, add them to the left
            if (current_player.team == local_team) {
                splash_data.left_chars[splash_data.ply_num_left] = current_player.c_kind;
                splash_data.left_colors[splash_data.ply_num_left] = current_player.costume;
                splash_data.ply_num_left++;
                OSReport("char id: %d\n", current_player.c_kind);
            }
            else {
                splash_data.right_chars[splash_data.ply_num_right] = current_player.c_kind;
                splash_data.right_colors[splash_data.ply_num_right] = current_player.costume;
                splash_data.ply_num_right++;
            }
        }
        OSReport("Ply num left: %d, Ply num right: %d\n", splash_data.ply_num_left, splash_data.ply_num_right);

    }
    else {
        splash_data.ply_num_left = 1;
        splash_data.ply_num_right = 1;
        splash_data.left_chars[0] = sss_data->match_init.playerData[0].c_kind;
        splash_data.left_colors[0] = sss_data->match_init.playerData[0].costume;
        splash_data.right_chars[0] = sss_data->match_init.playerData[1].c_kind;
        splash_data.right_colors[0] = sss_data->match_init.playerData[1].costume;
        OSReport("char id: %d\n", sss_data->match_init.playerData[0].c_kind);
        OSReport("char id2: %d\n", sss_data->match_init.playerData[1].c_kind);
    }

    *stc_splash_data = splash_data;

    // preload
    Preload *preload_table = Preload_GetTable();
    preload_table->queued.stage = sss_data->match_init.stage;
    // is there harm preloading all 4 slots?
    for (int i = 0; i < 4; i++) {
        preload_table->queued.fighters[i].kind = sss_data->match_init.playerData[i].c_kind;
        preload_table->queued.fighters[i].costume = sss_data->match_init.playerData[i].costume;
    }
    Preload_Update();
    // Splash_UnkPreload(199);
    // Splash_UnkPreload2(4);

    // // clear ssm queue
    Audio_ResetCache(0x1C);
    
    // load fighter audio
    // for (int i = 0; i < 4; i++) {

    // }

    HSD_Free(msrb);
    // OSReport("[SPLASH] heap free %.2fkb\n", (float)OSCheckHeap(HSD_GetHeapID()) / 1024);
    return;
}

void SlpSplash_Decide(MinorScene *minor_data) {
    OSReport("Splash Decide\n");
    Scene_SetNextMinor(SLP_SCENE_VS);
    return;
}

void SlpSplash_ColorOverwrite(MatchInit *match_data) {
    for (int i = 0; i < 4; i++) {
        u8 team = match_data->playerData[i].team;
        CharacterKind ckind = match_data->playerData[i].c_kind;
        u8 costume_index = Slippi_GetTeamCostumeIndex(team, ckind);
        match_data->playerData[i].costume = costume_index;
    }
    return;
}

#endif
