#ifndef SLIPPI_CUSTOM_H
#define SLIPPI_CUSTOM_H

#include "../../m-ex/MexTK/mex.h"
#include <stdbool.h>

#define R13_OFFSET_ISCUSTOM -0x4DA7 // some single button mode offset, hopefully unused elsewhere
#define CSS_CORNER_XTHRESH -20.f
#define CSS_CORNER_YTHRESH 22.f
#define TVO_HAS_PLAYED(chardata, index) ((chardata)->has_played & (1 << (index)))
#define TVO_SET_PLAYED(chardata, index) ((chardata)->has_played |= (1 << (index)))
#define TVO_CLEAR_PLAYED(chardata, index) ((chardata)->has_played &= ~(1 << (index)))

extern Text *text;

typedef struct MexMajorScene {
    u8 is_preload;
    u8 major_id;
    void *onLoad;
    void *onExit;
    void *onBoot;
    void *MinorScene; // array of minor scenes
    int x40;
} MexMajorScene;

typedef struct TvoCharacterData {
    u32 has_played; 
    CharacterKind last_played;
    u8 solo_player;
    bool match_success;
} TvoCharacterData;

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

bool IsCustomMode() {
    return R13_U8(R13_OFFSET_ISCUSTOM);
}

// int *css = (int *)0x80479D60;
u8 *stc_match_result = (u8 *)0x8046b6a8; // accessing this from stc_match doesnt work? its 2 bytes off?
TvoCharacterData *stc_tvo_characters = (TvoCharacterData *)0x803eadc8; // this is some debug strings for camera screenshot // size 0x3F

void Tvo_GetSoloPlayer(MatchInit data) {
    u8 team_counts[4] = {0};
    u8 team_players[4] = {0};
    u8 solo_player;

    for (int i = 0; i < 4; i++) {
        u8 team = data.playerData[i].team;
            team_counts[team]++;
            team_players[team] = i;

        // OSReport("player %d team %d\n", i, team);
    }

    // Check for teams with only one player
    for (int t = 0; t < 4; t++) {
        if (team_counts[t] == 1) {
            solo_player = team_players[t];
            stc_tvo_characters->solo_player = solo_player;
            // OSReport("Team %d has only player %d\n", t, solo_player);
        }
        if (team_counts[t] == 2) {
            stc_tvo_characters->solo_player = 6;
            // OSReport("No solo player found\n");
        }
    }
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

void Tvo_GetCharacter(CharacterKind last_played, CharacterKind *kind_out) {

    if (kind_out == (void *)0) {
        OSReport("null kind\n");
        return;
    }

    // exit early if desync/quit out
    if (!stc_tvo_characters->match_success) {
        *kind_out = last_played;
        return;
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
            new_kind = (CharacterKind)HSD_Randi(26);
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
    GrExternal stage_list[] = {
        GRKINDEXT_IZUMI, GRKINDEXT_PSTAD, GRKINDEXT_STORY,
        GRKINDEXT_BATTLE, GRKINDEXT_FD, GRKINDEXT_OLDPU 
    };

    const int stage_count = sizeof(stage_list) / sizeof(stage_list[0]);
    int random_idx = HSD_Randi(stage_count);
    *stage_out = stage_list[random_idx];
}

#endif
