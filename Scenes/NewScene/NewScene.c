#include "NewScene.h"
#include "Fighter.h"
#include "stage.h"
#include "../../Common.h"
#include "../../Files.h"
#include "../../Game/Sounds.h"
#include "../../Components/CardDoor.h"

static HSD_Archive *gui_archive;
static GUI_GameSetup *gui_assets;
static TurnsSetup_Data *data;

Text* text;


void minor_load() {
OSReport("New Scene minor load\n");

	// // Set up input handler. Initialize at top to make sure it runs before anything else
	GOBJ *input_handler_gobj = GObj_Create(4, 0, 128);
	GObj_AddProc(input_handler_gobj, InputsThink, 0);

	// Load file
	gui_archive = Archive_LoadFile("CustomSceneGUI2.dat");
	// gui_archive = Archive_LoadFile("GmTou1p.dat");
	gui_assets = Archive_GetPublicAddress(gui_archive, "SceneData");

	GOBJ *cam_gobj = GObj_Create(2, 3, 128);
	COBJ *cam_cobj = COBJ_LoadDesc(gui_assets->cobjs[0]);
	GObj_AddObject(cam_gobj, 1, cam_cobj);
	GOBJ_InitCamera(cam_gobj, CObjThink, 0);
	GObj_AddProc(cam_gobj, MainMenu_CamRotateThink, 5);

	// Indicates which gx_links to render
	cam_gobj->cobj_links = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4);

	// store cobj to static pointer, needed for MainMenu_CamRotateThink
	void **stc_cam_cobj = (R13 + (-0x4ADC));
	*stc_cam_cobj = gui_assets->cobjs[0];

	// create fog
	GOBJ *fog_gobj = GObj_Create(14, 2, 0);
	HSD_Fog *fog = Fog_LoadDesc(gui_assets->fog[0]);
	GObj_AddObject(fog_gobj, 4, fog);
	GObj_AddGXLink(fog_gobj, GXLink_Fog, 0, 128);

	// create lights
	GOBJ *light_gobj = GObj_Create(3, 4, 128);
	LOBJ *lobj = LObj_LoadAll(gui_assets->lights);
	GObj_AddObject(light_gobj, 2, lobj);
	GObj_AddGXLink(light_gobj, GXLink_LObj, 0, 128);

	// create background
	// JOBJ_LoadSet(0, gui_assets->jobjs[GUI_NewScene_JOBJ_Face], 0, 0, 3, 1, 1, GObj_Anim);

	// data->cd = CardDoor_Init(gui_assets);
	data = calloc(sizeof(TurnsSetup_Data));
	CardDoor *cd = CardDoor_Init(gui_assets);
	CardFace *fd = CardFace_Init(gui_assets);

	data->cd = cd;
	data->fd = fd;
	
	BGM_Play(10);

	text = Text_CreateText(0, 0);
	text->kerning = 1;
	text->align = 0;
	text->trans.Z = 0.0f;
	text->scale.X = 0.2f;
	text->scale.Y = 0.2f;
	text->color.r = 0;
	text->color.g = 100;
	text->color.b = 200;
	Text_AddSubtext(text, -125.0f, 5.0f, "hi");

	//////////////////////////////////
	Item_GlobalInit();
	Player_InitFigaTreeAllocData();
	FtJObjInfoInit();

	PlayerData *player1 = calloc(sizeof(PlayerData));
	PlayerData *player2 = calloc(sizeof(PlayerData));

	CharacterKind character = CKIND_BOY;
	CharacterKind character2 = CKIND_SANDBAG;
	FighterKind fighter = FTKIND_BOY;
	FighterKind fighter2 = FTKIND_SANDBAG;

	u8 costume = 0;
	u8 costume2 = 0;

	player1->c_kind = character;
	player1->p_kind = 3;
	player1->scale = 1.0f;

	player2->c_kind = character2;
	player2->p_kind = 3;
	player2->scale = 1.0f;

	MatchInit *match_data = VsModeEnterData;

	match_data->playerData[0] = *player1;
	match_data->playerData[1] = *player2;
	match_data->stage = 3;

	Preload* preload = Preload_GetTable();
	preload->queued.stage = 3;
	preload->queued.fighters[0].kind = character; // internal
	preload->queued.fighters[0].costume = costume;

	preload->queued.fighters[1].kind = character2;
	preload->queued.fighters[1].costume = costume2;
	Preload_Update();

	// StartMelee(VsModeEnterData);
	// TrainingMode_Init(VsModeEnterData);
	// VictoryScreen_Init2(VsModeEnterData);

	// SetupPlayerSlot(0);
	// SetupPlayerSlot(1);
	return;
}

void minor_think() {
	HSD_Pad* pad = PadGet(0, PADGET_ENGINE);

	if(pad->down & PAD_BUTTON_A) {
		SetupPlayerSlot(0);
	};

	if(pad->down & PAD_BUTTON_X) {
		GOBJ *fighter = Fighter_GetGObj(0);
		int player_id = Fighter_GetExternalID(0);
		FighterData *fd = fighter->userdata;
		int victory_pose = VictoryScreen_GetResultScreenAnimation(player_id);
		EnterVictoryAnimation(0, victory_pose);
		// ActionStateChange(1.0f, 1.0f, 1.0f, fighter, 307, 4, 0);
		// Fighter_EnterSleep(fighter, 600);
	}

	// if(pad->down & PAD_BUTTON_B) {
	// Scene_ExitMinor();
	// }

	// return;
}

void minor_exit() {
  OSReport("New Scene minor exit\n");
}

void load_kirby() {
  // ================== CHARACTER MODEL ====================
    FighterKind fighter = FTKIND_KIRBY;
    CharacterKind character = CKIND_KIRBY;

    int costume_id = 0;
    // Put character model in memory
    LoadCharacterJObjDesc(character, costume_id);
    JOBJDesc *charDesc = &(MODEL_INFO[character].costumes[costume_id]->desc);
    JOBJ *charJobj = JOBJ_LoadJoint(charDesc);

    charJobj->trans.X = 2.0f;
    charJobj->trans.Y = -6.0f;

    JOBJ_ReqAnimAll(charJobj, (u32) 0);
    JOBJ_AnimAll(charJobj);

    GOBJ *charGobj = GObj_Create(0x4, 0x5, 0x80);
    GObj_AddObject(charGobj, *objkind_jobj, charJobj);
    GObj_AddGXLink(charGobj, GXLink_Common, 1, 129);
}


// CardFace *CardFace_Init(GUI_GameSetup *gui) {
// 	CardFace *cf = calloc(sizeof(CardFace));

// 	// Init cf jobj
// 	cf->jobj_set = gui->jobjs[GUI_NewScene_JOBJ_Face];
// 	cf->gobj = JOBJ_LoadSet(0, cf->jobj_set, 0, 0, 3, 1, 1, GObj_Anim);
// 	cf->root_jobj = cf->gobj->hsd_object;
// 	// _SetDisplayState(cf);

// 	return cf;
// }

void ListPreloadFighters() {
    Preload* preload = Preload_GetTable(); // Get the preload table
    if (preload != NULL) {
        // Assuming we want to list fighters from the 'queued' table
        PreloadTable* table = &preload->queued;
        
        for (int i = 0; i < 8; ++i) { // Since there are 8 fighters in the array
            PreloadChar fighter = table->fighters[i];
            OSReport("Fighter %d: Kind = %d, Costume = %u\n", i, fighter.kind, fighter.costume);
        }
    } else {
        OSReport("Preload table not available.\n");
    }
}

void InputsThink(GOBJ *gobj) {
	HandleCardInputs();
}

void HandleCardInputs() {
  u8 port = R13_U8(-0x5108);
  u64 scrollInputs = Pad_GetRapidHeld(port);  // long delay between initial triggers, then frequent
  u64 downInputs = Pad_GetDown(port);
  CardDoor *cd = data->cd;
  CardFace *fd = data->fd;
  JOBJ *door = cd->door_jobjs[cd->door_index];

	if (scrollInputs & (HSD_BUTTON_RIGHT | HSD_BUTTON_DPAD_RIGHT)) {
		if (cd->door_index < 3) {
			cd->door_index++;
			fd->face_index++;
			// DoorStateChange(cd, cd->door_index);
			SFX_PlayCommon(CommonSound_NEXT);  // Play move SFX
		}

	} else if (scrollInputs & (HSD_BUTTON_LEFT | HSD_BUTTON_DPAD_LEFT)) {
		if (cd->door_index > 0) {
			cd->door_index--;
			fd->face_index--;
			SFX_PlayCommon(CommonSound_NEXT);  // Play move SFX
		}

	}
	// OSReport("DOOR JOBJ: %p\n", door);
}

void CObjThink(GOBJ *gobj) {
	COBJ *cobj = gobj->hsd_object;

	if (!CObj_SetCurrent(cobj)) {
		return;
  }

	// HSD_Fog *fog = stc_fog;
	CObj_SetEraseColor(0, 0, 0, 255);
	CObj_EraseScreen(cobj, 1, 0, 1);
	CObj_RenderGXLinks(gobj, 7);
	CObj_EndCurrent();
}
