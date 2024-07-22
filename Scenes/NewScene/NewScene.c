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
DevText* devtext;
COBJ* cam_cobj;
GOBJ* cam_gobj;

GOBJ* DrawingGObj;


u8 red[4] = {255, 0, 0, 255};
u8 green[4] = {0, 255, 0, 255};
u8 blue[4] = {0, 0, 255, 255};

GXColor purple = {0, 0, 255, 255};



void minor_load() {
OSReport("New Scene minor load\n");

	// // Set up input handler. Initialize at top to make sure it runs before anything else
	// GOBJ *input_handler_gobj = GObj_Create(4, 0, 128);
	// GObj_AddProc(input_handler_gobj, InputsThink, 0);
	DrawingGObj = GObj_Create(0x11, 24, 0);
	GObj_AddProc(DrawingGObj, drawing_think, 0);
	GObj_AddGXLink(DrawingGObj, create_drawing, 0x0, 128);

	// Load file
	// gui_archive = Archive_LoadFile("CustomSceneGUI2.dat");
	// gui_archive = Archive_LoadFile("GmTou1p.dat");
	// gui_assets = Archive_GetPublicAddress(gui_archive, "SceneData");

	// cam_gobj = GObj_Create(2, 3, 128);
	// cam_cobj = COBJ_LoadDesc(gui_assets->cobjs[0]);
	// GObj_AddObject(cam_gobj, 1, cam_cobj);
	// GOBJ_InitCamera(cam_gobj, CObjThink, 0);
	// GObj_AddProc(cam_gobj, MainMenu_CamRotateThink, 5);

	// Indicates which gx_links to render
	// cam_gobj->cobj_links = (1 << 0) + (1 << 1) + (1 << 2) + (1 << 3) + (1 << 4);

	// store cobj to static pointer, needed for MainMenu_CamRotateThink
	// void **stc_cam_cobj = (R13 + (-0x4ADC));
	// *stc_cam_cobj = gui_assets->cobjs[0];

	// create fog
	// GOBJ *fog_gobj = GObj_Create(14, 2, 0);
	// HSD_Fog *fog = Fog_LoadDesc(gui_assets->fog[0]);
	// GObj_AddObject(fog_gobj, 4, fog);
	// GObj_AddGXLink(fog_gobj, GXLink_Fog, 0, 128);

	// // create lights
	// GOBJ *light_gobj = GObj_Create(3, 4, 128);
	// LOBJ *lobj = LObj_LoadAll(gui_assets->lights);
	// GObj_AddObject(light_gobj, 2, lobj);
	// GObj_AddGXLink(light_gobj, GXLink_LObj, 0, 128);

	// create background
	// JOBJ_LoadSet(0, gui_assets->jobjs[GUI_NewScene_JOBJ_Background], 0, 0, 3, 1, 1, GObj_Anim);
	// JOBJ_LoadSet(0, gui_assets->jobjs[GUI_NewScene_JOBJ_MenuTop], 0, 0, 3, 1, 1, GObj_Anim);

	// // data->cd = CardDoor_Init(gui_assets);
	// data = calloc(sizeof(TurnsSetup_Data));
	// CardDoor *cd = CardDoor_Init(gui_assets);
	// CardFace *fd = CardFace_Init(gui_assets);

	// data->cd = cd;
	// data->fd = fd;
	
	BGM_Play(10);

	// text = Text_CreateText(0, 0);
	// text->kerning = 1;
	// text->align = 0;
	// text->trans.Z = 0.0f;
	// text->scale.X = 0.2f;
	// text->scale.Y = 0.2f;
	// text->color.r = 0;
	// text->color.g = 100;
	// text->color.b = 200;
	// Text_AddSubtext(text, -125.0f, 5.0f, "hello");

	u8 colour[4] = {0, 0, 0, 127};
	void *data = calloc(4096); // Allocate memory for one DevText structure
	devtext = DevelopText_CreateDataTable(3, 0, 20, 50, 10, data);
	DevelopText_Activate((void *) 0x804d6e1c, devtext);
	DevelopText_StoreBGColor(devtext, colour);
	DevelopText_ShowBG(devtext);
	// DevelopText_StoreTextScale(devtext, 8.0, 10.0);
	// DevelopText_ShowText(devtext);
	// devtext->show_cursor = 0;
	DevelopText_HideCursor(devtext);
	DevelopText_HideBG(devtext);
	DevelopText_HideText(devtext);
	//////////////////////////////////
	static void *fx;
	HSD_Archive *PlBx = MEX_LoadRelArchive("PlBx.dat", &fx, "ftDataBoy");
	OSReport("PlBx: %p\n", PlBx);
	// int Path = DVDConvertPathToEntrynum("PlBx.dat");
	// OSReport("Path: %p\n", Path);

	static HSD_Archive *out = 0x0;
	Archive_CheckPreloadAndGetSymbols(&out, "PlBx.dat", 0x8045992c, "ftDataBoy", 0);
	OSReport("Out: %p\n", out);

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
	
	static int teams_counts;
	int *DBLVL = (int *)0x804d4a08;
	*DBLVL = 3;
	VsMode_Enter(VsModeEnterData);
	// StartMelee(VsModeEnterData);
	// Match_InitPlayers();
	// TrainingMode_Init(VsModeEnterData);
	// VictoryScreen_Init2(VsModeEnterData);

	// SetupPlayerSlot(0);
	// SetupPlayerSlot(1);
	return;
}

void minor_think() {
	// DevelopMode_SceneUpdate((int *) 0x80479d6c);
	// OSReport("Test\n");
	// HSD_Pad* pad = PadGet(0, PADGET_ENGINE);
	// GOBJ* text_gobj = DevelopText_GetGObj();
	// // Vec3 cam_pos = {0.0f, 0.0f, 0.0f};
	// // CObjGetEyePosition(cam_cobj, &cam_pos);
	GOBJ* MainCameraGObj = Camera_LoadCameraEntity();
	COBJ* MainCameraCObj = MainCameraGObj->hsd_object;
	CObj_SetCurrent(MainCameraCObj);

	Vec3 interest = MainCameraCObj->interest->pos;
	float aspect = MainCameraCObj->projection_param.perspective.aspect;
	float fov = MainCameraCObj->projection_param.perspective.fov;
	float print_fov = fov * (M_PI / 180.0f);
	// float near = 1.0f;
	// float far = 1000.0f;
	Vec3 forward;
	Vec3 up;
	Vec3 left;
	Vec3 pos;

	CObjGetEyeVector(MainCameraCObj, &forward);
	CObjGetUpVector(MainCameraCObj, &up);
	CObjGetLeftVector(MainCameraCObj, &left);
	CObjGetEyePosition(MainCameraCObj, &pos);

	float pitch = atan2(forward.Y, -forward.Z);
	float yaw = atan2(-forward.X, -forward.Z);
	
	// Vec3 verts[4];
	// extract_frustum_near(fov, aspect, 100.0f, 10000.0f, &pos, &forward, &up, verts);

	// DrawStagePoints(type, 4, red);
	// DrawQuad(&corner1, &corner2, purple);
	// if (pad->substickX || pad->substickY) {
	// 	interest.X += pad->substickX;
	// 	interest.Y += pad->substickY;
	// 	CObjSetInterest(MainCameraCObj, &interest);

		// DevelopCam_OrbitCam(pad->substickX, pad->substickY, MainCameraGObj, &MainCameraCObj->eye_position->pos, &MainCameraCObj->interest->pos);
		DevelopText_EraseAllText(devtext);
	
		DevelopText_SetCursorXY(devtext, 0, 0);
		DevelopText_AddString(devtext, "Pos: ");
		DevelopText_SetCursorXY(devtext, 20, 0);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
	 	MainCameraCObj->eye_position->pos.X, MainCameraCObj->eye_position->pos.Y, MainCameraCObj->eye_position->pos.Z);
	
		DevelopText_SetCursorXY(devtext, 0, 1);
		DevelopText_AddString(devtext, "Interest: ");
		DevelopText_SetCursorXY(devtext, 20, 1);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		MainCameraCObj->interest->pos.X, MainCameraCObj->interest->pos.Y, MainCameraCObj->interest->pos.Z);

		DevelopText_SetCursorXY(devtext, 0, 2);
		DevelopText_AddString(devtext, "Forward: ");
		DevelopText_SetCursorXY(devtext, 20, 2);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		forward.X, forward.Y, forward.Z);

		DevelopText_SetCursorXY(devtext, 0, 3);
		DevelopText_AddString(devtext, "Up: ");
		DevelopText_SetCursorXY(devtext, 20, 3);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		up.X, up.Y, up.Z);

		DevelopText_SetCursorXY(devtext, 0, 4);
		DevelopText_AddString(devtext, "Left: ");
		DevelopText_SetCursorXY(devtext, 20, 4);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		left.X, left.Y, left.Z);

		DevelopText_SetCursorXY(devtext, 0, 6);
		DevelopText_AddString(devtext, "Pitch: ");
		DevelopText_SetCursorXY(devtext, 20, 6);
		DevelopText_AddString(devtext, "%03.2f\n",
		pitch * (180.0f / M_PI));

		DevelopText_SetCursorXY(devtext, 0, 7);
		DevelopText_AddString(devtext, "Yaw: ");
		DevelopText_SetCursorXY(devtext, 20, 7);
		DevelopText_AddString(devtext, "%03.2f\n",
		yaw * (180.0f / M_PI));
		

		// DevelopText_SetCursorXY(devtext, 0, 8);
		// DevelopText_AddString(devtext, "Top Left: ");
		// DevelopText_SetCursorXY(devtext, 20, 8);
		// DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		// verts[0].X, verts[0].Y, verts[0].Z);

	return;
	}

	// if(pad->down & PAD_BUTTON_A) {
	// 	int slot = HSD_Randi(3);
	// 	int	except[3] = {0};
	// 	bp();
	// 	store_except(except, slot);
	// 	bp();
	// };

	// if(pad->down & PAD_BUTTON_X) {
	// 	GOBJ *fighter = Fighter_GetGObj(0);
	// 	int player_id = Fighter_GetExternalID(0);
	// 	FighterData *fd = fighter->userdata;
	// 	int victory_pose = VictoryScreen_GetResultScreenAnimation(player_id);
	// 	EnterVictoryAnimation(0, victory_pose);
	// 	// ActionStateChange(1.0f, 1.0f, 1.0f, fighter, 307, 4, 0);
	// 	// Fighter_EnterSleep(fighter, 600);
	// }

	// if(pad->down & PAD_BUTTON_B) {
	// Scene_ExitMinor();
	// }

// }

void minor_exit() {
  OSReport("New Scene minor exit\n");
}


// int CountTeam(void) {
//     int team_counts[4] = {0}; // Array to hold the count of players in each team

//     // Count each player's team
//     for (int slot = 0; slot < 4; slot++) {
//         unsigned char team = Fighter_GetTeam(slot);
//         if (team < 4) {
//             team_counts[team]++;
//         }
//     }

//     // Print team counts (optional, for verification)
//     for (int team = 0; team < 4; team++) {
//         OSReport("Team %d has %d players\n", team, team_counts[team]);
//     }

//     // If you need to find a specific condition (e.g., a team with exactly 3 players)
//     for (int team = 0; team < 4; team++) {
//         if (team_counts[team] == 3) {
//             return team; // Returns the first team with exactly 3 players
//         }
//     }

//     return -1; // Return -1 if no team has exactly 3 players
// }

// void store_except(int except[], int slot) {
//     int index = 0;
//     for (int i = 0; i <= 3; i++) {
//         if (i != slot) {
//             except[index++] = i;
//         }
//     }
// }

void create_drawing(GOBJ *gobj) {
	Mtx mtx;
	GOBJ* MainCameraGObj = Camera_LoadCameraEntity();
	COBJ* cobj = MainCameraGObj->hsd_object;
	Setup2DDrawing(10.0f, 10.0f, 10);
	u32 clr = 0x00E4FFFF;
	HSD_LObjSetupInit(cobj);
	GXSetCullMode(0);
	// GXClearVtxDesc();
	// GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	// GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	cobj = COBJ_GetCurrent();
	COBJ_GetViewingMtx(cobj, &mtx);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(mtx, 0);
	GXSetZMode(0, 0, 0);
	GXSetLineWidth(0x10, 0);
	StateSetPointSize(0xFF, 0);
	
	Vec3 interest = cobj->interest->pos;
	float aspect = cobj->projection_param.perspective.aspect;
	float fov = cobj->projection_param.perspective.fov;
	// fov = fov * (M_PI / 180.0f);
	// float near = 1000.0f;
	// float far = 1000.0f;
	Vec3 forward;
	Vec3 up;
	Vec3 left;
	Vec3 pos;

	CObjGetEyeVector(cobj, &forward);
	CObjGetUpVector(cobj, &up);
	CObjGetLeftVector(cobj, &left);
	CObjGetEyePosition(cobj, &pos);

	Vec3 near_plane[4];
	extract_frustum_near(fov, aspect, 10.f, 1000.f, &pos, &forward, &up, near_plane);
	Vec3 far_plane[4];
	extract_frustum_far(fov, aspect, 100.f, 1000.f, &pos, &forward, &up, far_plane);

	// SetDrawColor(red);
	GXClearVtxDesc();
	// GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	// GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	// GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
	// GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	// GXSetLineWidth(0x10, 0);

	// GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
	// GXPosition3f32(near_plane[0].X, near_plane[0].Y, near_plane[0].Z);
	// GXColor3u8( 0xff, 0x00, 0x00 );
	// GXPosition3f32(far_plane[0].X, far_plane[0].Y, far_plane[0].Z);
	// GXColor3u8( 0x44, 0x44, 0xff );

	// GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
	// GXPosition3f32(near_plane[1].X, near_plane[1].Y, near_plane[1].Z);
	// GXColor3u8( 0xff, 0x00, 0x00 );
	// GXPosition3f32(far_plane[1].X, far_plane[1].Y, far_plane[1].Z);
	// GXColor3u8( 0x44, 0x44, 0xff );

	// GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
	// GXPosition3f32(near_plane[2].X, near_plane[2].Y, near_plane[2].Z);
	// GXColor3u8( 0xff, 0x00, 0x00 );
	// GXPosition3f32(far_plane[2].X, far_plane[2].Y, far_plane[2].Z);
	// GXColor3u8( 0x44, 0x44, 0xff );

	// GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
	// GXPosition3f32(near_plane[3].X, near_plane[3].Y, near_plane[3].Z);
	// GXColor3u8( 0xff, 0x00, 0x00 );
	// GXPosition3f32(far_plane[3].X, far_plane[3].Y, far_plane[3].Z);
	// GXColor3u8( 0x44, 0x44, 0xff );

	// GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
	// GXPosition3f32(near_plane[0].X, near_plane[0].Y, near_plane[0].Z);
	// GXColor3u8( 0xff, 0x00, 0x00 );
	// GXPosition3f32(near_plane[1].X, near_plane[1].Y, near_plane[1].Z);
	// GXColor3u8( 0x00, 0xff, 0x00 );
	// GXPosition3f32(near_plane[2].X, near_plane[2].Y, near_plane[2].Z);
	// GXColor3u8( 0x00, 0x00, 0xff);
	// GXPosition3f32(near_plane[3].X, near_plane[3].Y, near_plane[3].Z);
	// GXColor3u8( 0xff, 0xff, 0x00 );
	// GXPosition3f32(near_plane[0].X, near_plane[0].Y, near_plane[0].Z);
	// GXColor3u8( 0xff, 0x00, 0x00 );

	// GXSetLineWidth(0x20, 0);
	// GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
	// GXPosition3f32(far_plane[0].X, far_plane[0].Y, far_plane[0].Z);
	// GXColor3u8( 0x44, 0x44, 0xff );
	// GXPosition3f32(far_plane[1].X, far_plane[1].Y, far_plane[1].Z);
	// GXColor3u8( 0xff, 0x00, 0xff);
	// GXPosition3f32(far_plane[2].X, far_plane[2].Y, far_plane[2].Z);
	// GXColor3u8( 0xff, 0xff, 0xf );
	// GXPosition3f32(far_plane[3].X, far_plane[3].Y, far_plane[3].Z);
	// GXColor3u8( 0x00, 0xff, 0x00 );
	// GXPosition3f32(far_plane[0].X, far_plane[0].Y, far_plane[0].Z);
	// GXColor3u8( 0x44, 0x44, 0xff );


}

void drawing_think(GOBJ *gobj) {
	// OSReport("Drawing think\n");
}


void extract_frustum_near(float fov, float aspect, float near, float far, Vec3 *position, Vec3 *forward, Vec3 *up, Vec3* vertices) {
    float fov_radians = (fov * M_PI) / 180.0;  // Convert degrees to radians

    float half_height_near = tanf(fov_radians / 2) * near;
    float half_width_near = half_height_near * aspect;

    float half_height_far = tanf(fov_radians / 2) * far;
    float half_width_far = half_height_far * aspect;

    Vec3 temp, cross;

    // Calculate cross product of forward and up vectors
    VECCrossProduct(forward, up, &cross);

	///////////////////////////////////////////////////////////////////////////////////////////
	// NEAR
    // Top left near
    VECScale(forward, &temp, near);
    VECAdd(position, &temp, &vertices[0]);
    VECScale(up, &temp, half_height_near);
    VECAdd(&vertices[0], &temp, &vertices[0]);
    VECScale(&cross, &temp, -half_width_near);
    VECAdd(&vertices[0], &temp, &vertices[0]);
	

    // Top right near
    VECScale(&cross, &temp, 2 * half_width_near);
    VECAdd(&vertices[0], &temp, &vertices[1]);

	// Bottom right near
	VECScale(forward, &temp, near);
	VECAdd(position, &temp, &vertices[2]);
	VECScale(up, &temp, -half_height_near);
	VECAdd(&vertices[2], &temp, &vertices[2]);
	VECScale(&cross, &temp, half_width_near);
	VECAdd(&vertices[2], &temp, &vertices[2]);

	// Bottom left near
	VECScale(&cross, &temp, -2 * half_width_near);
	VECAdd(&vertices[2], &temp, &vertices[3]);

	// for (int i = 0; i < 8; i++) {
	// 	OSReport("Vert: %d X: %f, Y: %f, Z: %f\n", i, vertices[i].X, vertices[i].Y, vertices[i].Z);
	// }
}


void extract_frustum_far(float fov, float aspect, float near, float far, Vec3 *position, Vec3 *forward, Vec3 *up, Vec3* vertices) {
    float fov_radians = (fov * M_PI) / 180.0;  // Convert degrees to radians

    float half_height_near = tanf(fov_radians / 2) * near;
    float half_width_near = half_height_near * aspect;

    float half_height_far = tanf(fov_radians / 2) * far;
    float half_width_far = half_height_far * aspect;

    Vec3 temp, cross;

    // Calculate cross product of forward and up vectors
    VECCrossProduct(forward, up, &cross);

	///////////////////////////////////////////////////////////////////////////////////////////
	// NEAR
    // Top left near
    VECScale(forward, &temp, near);
    VECAdd(position, &temp, &vertices[0]);
    VECScale(up, &temp, half_height_near);
    VECAdd(&vertices[0], &temp, &vertices[0]);
    VECScale(&cross, &temp, -half_width_near);
    VECAdd(&vertices[0], &temp, &vertices[0]);
	

    // Top right near
    VECScale(&cross, &temp, 2 * half_width_near);
    VECAdd(&vertices[0], &temp, &vertices[1]);

	// Bottom right near
	VECScale(forward, &temp, near);
	VECAdd(position, &temp, &vertices[2]);
	VECScale(up, &temp, -half_height_near);
	VECAdd(&vertices[2], &temp, &vertices[2]);
	VECScale(&cross, &temp, half_width_near);
	VECAdd(&vertices[2], &temp, &vertices[2]);

	// Bottom left near
	VECScale(&cross, &temp, -2 * half_width_near);
	VECAdd(&vertices[2], &temp, &vertices[3]);
}

// void extract_frustum_far(float fov, float aspect, float near, float far, Vec3 *position, Vec3 *forward, Vec3 *up, Vec3* vertices) {
//     float fov_radians = (fov * M_PI) / 180.0;  // Convert degrees to radians

//     float half_height_near = tanf(fov_radians / 2) * near;
//     float half_width_near = half_height_near * aspect;

//     float half_height_far = tanf(fov_radians / 2) * far;
//     float half_width_far = half_height_far * aspect;

//     Vec3 temp, cross;

//     // Calculate cross product of forward and up vectors
//     VECCrossProduct(forward, up, &cross);

//     // Normalize the cross vector
//     VECNormalize(&cross, &cross);
//     Vec3 forward_norm;
//     // Normalize the forward vector
//     VECNormalize(forward, &forward_norm);

//     Vec3 up_norm;
//     // Normalize the up vector
//     VECNormalize(up, &up_norm);

//     ///////////////////////////////////////////////////////////////////////////////////////////
//     // FAR PLANE
//     // Top left far
//     VECScale(&forward_norm, &temp, far);
//     VECAdd(position, &temp, &vertices[0]);
//     VECScale(&up_norm, &temp, half_height_near);
//     VECAdd(&vertices[0], &temp, &vertices[0]);
//     VECScale(&cross, &temp, -half_width_near);
//     VECAdd(&vertices[0], &temp, &vertices[0]);

//     // Top right far
//     VECScale(&cross, &temp, 2 * half_width_near);
//     VECAdd(&vertices[0], &temp, &vertices[1]);

//     // Bottom right far
//     VECScale(&forward_norm, &temp, far);
//     VECAdd(position, &temp, &vertices[2]);
//     VECScale(&up_norm, &temp, -half_height_near);
//     VECAdd(&vertices[2], &temp, &vertices[2]);
//     VECScale(&cross, &temp, half_width_near);
//     VECAdd(&vertices[2], &temp, &vertices[2]);

//     // Bottom left farfar
//     VECScale(&cross, &temp, -2 * half_width_near);
//     VECAdd(&vertices[2], &temp, &vertices[3]);

// 	// for (int i = 0; i < 4; i++) {
// 	// 	OSReport("Vert: %d X: %f, Y: %f, Z: %f\n", i, vertices[i].X, vertices[i].Y, vertices[i].Z);
// 	// }
// }


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

