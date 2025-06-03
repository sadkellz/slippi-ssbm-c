#ifndef SLIPPI_CSS_MAIN_C
#define SLIPPI_CSS_MAIN_C

#include "Main.h"

#include "../../Core/Notifications/Notifications.c"
#include "Chat/Chat.c"
#include "../3v1/CustomGame.h"

// static Text* text;
unsigned char copy_text[] = { 
    0x81, 0x69,
    0x82, 0x83,
    0x81, 0x6A, // (C)
    ' ', 'K', 'E', 'L', 'L', 'Z', ' ', '2', '0', '2', '5' //  how the fuck do i combine these
};


void UpdateOnlineCSS() {
    if (!IsSlippiOnlineCSS()) return;
    //bp();

    // Do not handle notifications or chat input if on Name Entry Screen
    if (IsOnCSSNameEntryScreen()) return;

    ListenForNotifications();
    ListenForChatInput();
}

void InitOnlineCSS() {
    if (!IsSlippiOnlineCSS()) return;

    InitChatMessages();
}


// 3v1

void SetMatchSelections(u8 char_id, u8 char_color, u8 char_option, u16 stage_id, u8 stage_option, u8 team_id) {
    ExiSlippi_SetSelections_Query *ssq = calloc(sizeof(ExiSlippi_SetSelections_Query));
    ssq->command = ExiSlippi_Command_SET_MATCH_SELECTIONS;
    ssq->team_id = team_id;
    ssq->char_id = char_id;
    ssq->char_color_id = char_color;
    ssq->char_option = char_option;
    ssq->stage_id = stage_id;
    ssq->stage_option = stage_option;
    ssq->online_mode = -1;  // 0 is Ranked. Doesn't actually do anything in Dolphin atm
    ExiSlippi_Transfer(ssq, sizeof(ExiSlippi_SetSelections_Query), ExiSlippi_TransferMode_WRITE);
    HSD_Free(ssq);
}


void SetupCustomCSS() {
	GXColor color = {240, 240, 240, 255};
	GXColor green = {0, 255, 0, 255};

   	Text_CreateCanvas(0, 0, 9, 13, 0, 14, 0, 19);
	text = Text_CreateText(0, 0);
	text->kerning = 1;
	text->align = 0;
	text->trans = (Vec3){0.f, -20.f, 0.f};
	text->scale = (Vec2){0.08f, 0.08f};
	text->color = color;
	// text->viewport_color = color2;
	// text->aspect = (Vec2){640.f, 60.f};
	int header = Text_AddSubtext(text, -80.0f, -44.0f, "3v1 IRON MAN");
	int copyright = Text_AddSubtext(text, 250.0f, 540.f, copy_text);
    Text_SetScale(text, copyright, 0.5f, 0.5f);
    Text_SetColor(text, copyright, &green);
}

void CheckForModeSwitch() {
    Vec2 puck_pos;
    puck_pos = stc_css_cursors[*stc_css_singeplyport]->pos;
    bool is_custom_mode = R13_U8(R13_OFFSET_ISCUSTOM);
    if (IsOnCSSNameEntryScreen()) { text = NULL;}

    if (puck_pos.X < CSS_CORNER_XTHRESH && puck_pos.Y > CSS_CORNER_YTHRESH) {
        // OSReport("pos: %f, %f\n", puck_pos.X, puck_pos.Y);
        HSD_Pad* pad = PadGet(*stc_css_singeplyport, PADGET_ENGINE);
        if (pad->down & PAD_BUTTON_A) {
            // switch mode
            
            is_custom_mode = !is_custom_mode;
            R13_U8(R13_OFFSET_ISCUSTOM) = is_custom_mode;

            if (is_custom_mode) {
                SetupCustomCSS();
            }
            else {
                Text_Destroy(text);
                Text_DestroyAllAlloc(text);
                text = NULL;
            }
        }
    }

    if (is_custom_mode && !text && !IsOnCSSNameEntryScreen()) {
        SetupCustomCSS();
    }
}

#endif SLIPPI_CSS_MAIN_C
