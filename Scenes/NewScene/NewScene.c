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
static PIPData *pip_data;

Text* text;
DevText* devtext;
COBJ* cam_cobj;
GOBJ* cam_gobj;

GOBJ* RenderTargetCam;

GOBJ* DrawingGObj;
static HSD_Archive *results_archive;

u8 red[4] = {255, 0, 0, 255};
u8 green[4] = {0, 255, 0, 255};
u8 blue[4] = {0, 0, 255, 255};

GXColor purple = {0, 0, 255, 255};

GUI_GameSetup* results_pnl;

static JOBJ *GetNthSibling(JOBJ *start, int n) {
    while (start != NULL && n-- > 0) {
        start = start->sibling;
    }
    return start;
}

void minor_load() {
OSReport("New Scene minor load\n");

	// // Set up input handler. Initialize at top to make sure it runs before anything else
	// GOBJ *input_handler_gobj = GObj_Create(4, 0, 128);
	// GObj_AddProc(input_handler_gobj, InputsThink, 0);

	GOBJ *rst_gobj = GObj_Create(14, 15, 0);

	// Load results
	results_archive = Archive_LoadFile("GmRst.usd");
	results_pnl = Archive_GetPublicAddress(results_archive, "pnlsce");
	JOBJ* root_joint = JOBJ_LoadJoint(results_pnl->jobjs[0]->jobj);
	OSReport("root_joint: %p\n", root_joint);

	// Add GX link to render target
	GObj_AddObject(rst_gobj, 0x3, root_joint);
	GObj_AddGXLink(rst_gobj, (void *)0x80175038, 11, 0);
	JOBJ_SetFlagsAll(root_joint, JOBJ_HIDDEN);


	JOBJ* GX_JOBJ = GetNthSibling(root_joint->child, 17);
	OSReport("GX_JOBJ: %p\n", GX_JOBJ);
	// Unhide only the GX_JOBJ
	JOBJ_ClearFlagsAll(GX_JOBJ->child, JOBJ_HIDDEN);
	// Hide insignia graphic
	DOBJ* GX_DOBJ = GX_JOBJ->child->dobj;
	OSReport("GX_DOBJ: %p\n", GX_DOBJ);
	DOBJ_SetFlags(GX_DOBJ, DOBJ_HIDDEN);

	OSReport("test: %p\n", GX_JOBJ);
	JOBJ_ReqAnim(root_joint, 0);
	JOBJ_ReqAnimAll(root_joint, 0);
	JOBJ_AnimAll(root_joint);
	// center the GX_JOBJ
	GX_JOBJ->child->trans.X = 0.0f;
	GX_JOBJ->child->trans.Y = 7.0f;
	GX_JOBJ->child->trans.Z = 50.0f;
	GX_JOBJ->child->scale.X = 1.0f;
	GX_JOBJ->child->scale.Y = 1.0f;
	GX_JOBJ->child->scale.Z = 1.0f;
	
	DOBJ* GX_FRAME = GX_JOBJ->child->dobj->next->next;
	DOBJ* GX_MAIN = GX_JOBJ->child->dobj->next;
	GX_JOBJ->child->scale.X = 1.0f;




	pip_data = calloc(52);
	_HSD_ImageDesc* GX_IMAGE = GX_MAIN->mobj->tobj->imagedesc;
	GX_IMAGE->width = 150;
	GX_IMAGE->height = 150;
	pip_data->image = GX_IMAGE;
	pip_data->texture_mtx = &GX_MAIN->mobj->tobj->mtx;
	GX_AllocImageData(GX_IMAGE->img_ptr, 150, 150, 5, 2559);


	// create lights
	// GOBJ *light_gobj = GObj_Create(3, 4, 128);
	// LOBJ *lobj = LObj_LoadAll(results_pnl->lights);
	// GObj_AddObject(light_gobj, 2, lobj);
	// GObj_AddGXLink(light_gobj, GXLink_LObj, 0, 128);

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
	devtext = DevelopText_CreateDataTable(3, 100, 0, 50, 18, data);
	DevelopText_Activate((void *) 0x804d6e1c, devtext);
	DevelopText_StoreBGColor(devtext, colour);
	DevelopText_ShowBG(devtext);
	DevelopText_StoreTextScale(devtext, 8.0, 10.0);
	DevelopText_ShowText(devtext);
	// devtext->show_cursor = 0;
	DevelopText_HideCursor(devtext);
	// DevelopText_HideBG(devtext);
	// DevelopText_HideText(devtext);
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
	CreateCamera();
	return;
}

void minor_think() {
	HSD_Pad* pad = PadGet(0, PADGET_ENGINE);

	GOBJ* MainCameraGObj = Camera_LoadCameraEntity();
	COBJ* MainCameraCObj = MainCameraGObj->hsd_object;
	// CObj_SetCurrent(MainCameraCObj);

	Vec3 interest = MainCameraCObj->interest->pos;
	float aspect = MainCameraCObj->projection_param.perspective.aspect;
	float fov = MainCameraCObj->projection_param.perspective.fov;
	// float print_fov = fov * (M_PI / 180.0f);
	float near = 1.0f;
	float far = 1000.0f;
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

///////////////////////////////////////////////////////////////////////////////////////////	
		DevelopText_EraseAllText(devtext);
		DevelopText_SetCursorXY(devtext, 5, 0);
		DevelopText_AddString(devtext, "Main Camera\n");

		DevelopText_SetCursorXY(devtext, 0, 1);
		DevelopText_AddString(devtext, "Pos: ");
		DevelopText_SetCursorXY(devtext, 20, 1);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
	 	MainCameraCObj->eye_position->pos.X, MainCameraCObj->eye_position->pos.Y, MainCameraCObj->eye_position->pos.Z);
	
		DevelopText_SetCursorXY(devtext, 0, 2);
		DevelopText_AddString(devtext, "Interest: ");
		DevelopText_SetCursorXY(devtext, 20, 2);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		MainCameraCObj->interest->pos.X, MainCameraCObj->interest->pos.Y, MainCameraCObj->interest->pos.Z);

		DevelopText_SetCursorXY(devtext, 0, 3);
		DevelopText_AddString(devtext, "Forward: ");
		DevelopText_SetCursorXY(devtext, 20, 3);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		forward.X, forward.Y, forward.Z);

		DevelopText_SetCursorXY(devtext, 0, 4);
		DevelopText_AddString(devtext, "Up: ");
		DevelopText_SetCursorXY(devtext, 20, 4);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		up.X, up.Y, up.Z);

		DevelopText_SetCursorXY(devtext, 0, 5);
		DevelopText_AddString(devtext, "Left: ");
		DevelopText_SetCursorXY(devtext, 20, 5);
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
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////	
		MainCameraCObj = RenderTargetCam->hsd_object;
		CObjGetEyeVector(MainCameraCObj, &forward);
		CObjGetUpVector(MainCameraCObj, &up);
		CObjGetLeftVector(MainCameraCObj, &left);
		CObjGetEyePosition(MainCameraCObj, &pos);
		interest = MainCameraCObj->interest->pos;

		pitch = atan2(forward.Y, -forward.Z);
		yaw = atan2(-forward.X, -forward.Z);
		DevelopText_SetCursorXY(devtext, 5, 9);
		DevelopText_AddString(devtext, "Render Camera\n");

		DevelopText_SetCursorXY(devtext, 0, 10);
		DevelopText_AddString(devtext, "Pos: ");
		DevelopText_SetCursorXY(devtext, 20, 10);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
	 	MainCameraCObj->eye_position->pos.X, MainCameraCObj->eye_position->pos.Y, MainCameraCObj->eye_position->pos.Z);
	
		DevelopText_SetCursorXY(devtext, 0, 11);
		DevelopText_AddString(devtext, "Interest: ");
		DevelopText_SetCursorXY(devtext, 20, 11);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		MainCameraCObj->interest->pos.X, MainCameraCObj->interest->pos.Y, MainCameraCObj->interest->pos.Z);

		DevelopText_SetCursorXY(devtext, 0, 12);
		DevelopText_AddString(devtext, "Forward: ");
		DevelopText_SetCursorXY(devtext, 20, 12);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		forward.X, forward.Y, forward.Z);

		DevelopText_SetCursorXY(devtext, 0, 13);
		DevelopText_AddString(devtext, "Up: ");
		DevelopText_SetCursorXY(devtext, 20, 13);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		up.X, up.Y, up.Z);

		DevelopText_SetCursorXY(devtext, 0, 14);
		DevelopText_AddString(devtext, "Left: ");
		DevelopText_SetCursorXY(devtext, 20, 14);
		DevelopText_AddString(devtext, "%03.2f, %03.2f, %03.2f\n",
		left.X, left.Y, left.Z);

		DevelopText_SetCursorXY(devtext, 0, 15);
		DevelopText_AddString(devtext, "Pitch: ");
		DevelopText_SetCursorXY(devtext, 20, 15);
		DevelopText_AddString(devtext, "%03.2f\n",
		pitch * (180.0f / M_PI));

		DevelopText_SetCursorXY(devtext, 0, 16);
		DevelopText_AddString(devtext, "Yaw: ");
		DevelopText_SetCursorXY(devtext, 20, 16);
		DevelopText_AddString(devtext, "%03.2f\n",
		yaw * (180.0f / M_PI));

		DevelopText_SetCursorXY(devtext, 0, 17);
		DevelopText_AddString(devtext, "FOV: ");
		DevelopText_SetCursorXY(devtext, 20, 17);
		DevelopText_AddString(devtext, "%03.2f\n",
		MainCameraCObj->projection_param.perspective.fov);

///////////////////////////////////////////////////////////////////////////////////////////

	// if (pad->down & PAD_BUTTON_B) {
	// 		if (DrawingGObj == NULL) {
	// 			DrawingGObj = GObj_Create(0x11, 24, 0);
	// 			GObj_AddProc(DrawingGObj, drawing_think, 0);
	// 			GObj_AddGXLink(DrawingGObj, create_drawing, 0x0, 128);
	// 		}
	// 	}

	if (pad->down & PAD_BUTTON_A) {
			// Camera_SetFixed();
			// Match_SetStageRenderFlags(0);
			CObj_SetCurrent(RenderTargetCam->hsd_object);
			CObj_UpdateFromCamera(RenderTargetCam);
			
		}

	if (pad->down & PAD_BUTTON_X) {
			DevelopText_HideText(devtext);
			DevelopText_HideBG(devtext);
		}
	if (pad->down & PAD_BUTTON_Y) {
			DevelopText_ShowText(devtext);
			DevelopText_ShowBG(devtext);
		}

	if (pad->down & PAD_BUTTON_B) {
			Vec3 start = {0.0f, 10.0f, 0.0f};
			Vec3 end = {110.0f, 0.0f, 110.0f};
			float radius = 100.0f;
			Develop_DrawSphere(radius, &start, &end, &purple, &purple);
		}

	return;
	}


void minor_exit() {
  OSReport("New Scene minor exit\n");
}


static inline void normalToColor(f32 NX, f32 NY, f32 NZ, u8 *r, u8 *g, u8 *b) {
    f32 length = sqrtf(NX * NX + NY * NY + NZ * NZ);
    NX /= length;
    NY /= length;
    NZ /= length;

    *r = (u8)((NX + 1.0f) * 127.5f);
    *g = (u8)((NY + 1.0f) * 127.5f);
    *b = (u8)((NZ + 1.0f) * 127.5f);
}

// Function to calculate vertex normal
static void calculateNormal(Vec3 v1, Vec3 v2, Vec3 v3, f32 *NX, f32 *NY, f32 *NZ) {
    f32 Ux = v2.X - v1.X;
    f32 Uy = v2.Y - v1.Y;
    f32 Uz = v2.Z - v1.Z;

    f32 Vx = v3.X - v1.X;
    f32 Vy = v3.Y - v1.Y;
    f32 Vz = v3.Z - v1.Z;

    *NX = Uy * Vz - Uz * Vy;
    *NY = Uz * Vx - Ux * Vz;
    *NZ = Ux * Vy - Uy * Vx;

    f32 length = sqrtf((*NX) * (*NX) + (*NY) * (*NY) + (*NZ) * (*NZ));
    *NX /= length;
    *NY /= length;
    *NZ /= length;
}

void create_drawing(GOBJ *gobj) {
	// if (NewCam == NULL) return;

	Mtx mtx;
	GOBJ* MainCameraGObj = Camera_LoadCameraEntity();
	COBJ* cobj = MainCameraGObj->hsd_object;
	// COBJ* cobj = NewCam->hsd_object;
	Setup2DDrawing(10.0f, 10.0f, 10);
	u32 clr = 0x00E4FFFF;
	HSD_LObjSetupInit(cobj);
	GXSetCullMode(0);
	// GXClearVtxDesc();
	// GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	// GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	// cobj = COBJ_GetCurrent();
	COBJ_GetViewingMtx(cobj, &mtx);
	GXSetCurrentMtx(0);
	GXLoadPosMtxImm(mtx, 0);
	GXSetZMode(1, 3, 1);
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
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetLineWidth(0x10, 0);

    u8 r, g, b;
    f32 NX, NY, NZ;

    // Drawing near plane quad (normal pointing towards the viewer)
    calculateNormal(near_plane[0], near_plane[1], near_plane[2], &NX, &NY, &NZ);
    normalToColor(NX, NY, NZ, &r, &g, &b);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    for (int i = 0; i < 4; ++i) {
        GXPosition3f32(near_plane[i].X, near_plane[i].Y, near_plane[i].Z);
        GXColor3u8(r, g, b);
    }

    // Drawing far plane quad (normal pointing away from the viewer)
    calculateNormal(far_plane[0], far_plane[1], far_plane[2], &NX, &NY, &NZ);
    normalToColor(NX, NY, NZ, &r, &g, &b);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    for (int i = 0; i < 4; ++i) {
        GXPosition3f32(far_plane[i].X, far_plane[i].Y, far_plane[i].Z);
        GXColor3u8(r, g, b);
    }

    // Drawing connecting quads between near and far planes
    for (int i = 0; i < 4; ++i) {
        int next = (i + 1) % 4;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        // Calculate normals for each quad
        calculateNormal(near_plane[i], near_plane[next], far_plane[next], &NX, &NY, &NZ);
        normalToColor(NX, NY, NZ, &r, &g, &b);

        GXPosition3f32(near_plane[i].X, near_plane[i].Y, near_plane[i].Z);
        GXColor3u8(r, g, b);

        GXPosition3f32(near_plane[next].X, near_plane[next].Y, near_plane[next].Z);
        GXColor3u8(r, g, b);

        GXPosition3f32(far_plane[next].X, far_plane[next].Y, far_plane[next].Z);
        GXColor3u8(r, g, b);

        GXPosition3f32(far_plane[i].X, far_plane[i].Y, far_plane[i].Z);
        GXColor3u8(r, g, b);
    }

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

// void HandleCardInputs() {
//   u8 port = R13_U8(-0x5108);
//   u64 scrollInputs = Pad_GetRapidHeld(port);  // long delay between initial triggers, then frequent
//   u64 downInputs = Pad_GetDown(port);
//   CardDoor *cd = data->cd;
//   CardFace *fd = data->fd;
//   JOBJ *door = cd->door_jobjs[cd->door_index];

// 	if (scrollInputs & (HSD_BUTTON_RIGHT | HSD_BUTTON_DPAD_RIGHT)) {
// 		if (cd->door_index < 3) {
// 			cd->door_index++;
// 			fd->face_index++;
// 			// DoorStateChange(cd, cd->door_index);
// 			SFX_PlayCommon(CommonSound_NEXT);  // Play move SFX
// 		}

// 	} else if (scrollInputs & (HSD_BUTTON_LEFT | HSD_BUTTON_DPAD_LEFT)) {
// 		if (cd->door_index > 0) {
// 			cd->door_index--;
// 			fd->face_index--;
// 			SFX_PlayCommon(CommonSound_NEXT);  // Play move SFX
// 		}

// 	}
	// OSReport("DOOR JOBJ: %p\n", door);
// }

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

GOBJ* CreateCamera() {
	RenderTargetCam = GObj_Create(0x13, 0x14, 0);
	// COBJDesc* desc = calloc(sizeof(COBJDesc));

	// COBJ_Init(desc, (COBJDesc *) 0x803bcb64);
	COBJ* cobj = COBJ_LoadDesc((COBJDesc *) 0x803bcb64);
	GObj_AddObject(RenderTargetCam, (u8) 0x803e0f34, cobj);
	// GOBJ_InitCamera(RenderTargetCam, (void *) 0x803910d8, 8);
	GOBJ_InitCamera(RenderTargetCam, UpdateRenderTarget, 2);
	cobj->projection_param.perspective.fov = 80.f;
	// GObj_AddProc(NewCam, (void *) 0x8002ddc4, 0x12);
	GOBJ* MainCameraGObj = Camera_LoadCameraEntity();
	OSReport("MainCameraGObj: %p\n", MainCameraGObj);
	OSReport("RenderTargetCam: %p\n", RenderTargetCam);
	return RenderTargetCam;
	}

void UpdateRenderTarget(GOBJ* gobj) {
	PIPData *img = pip_data;
	COBJ* cobj = gobj->hsd_object;
	Mtx* img_mtx = pip_data->texture_mtx;
	Mtx* b;
	Mtx* mtx;
	
	GOBJ* MainCameraGObj = Camera_LoadCameraEntity();
	u8 (*color)[4] = (u8 (*)[4])0x80452c70; // MainCamera bg color
	// OSReport("colors: %x, %x, %x, %x\n", (*color)[0], (*color)[1], (*color)[2], (*color)[3]);
	if (CObj_SetCurrent(cobj))
	{
		// MainCameraGObj->cobj_links = (0 << 0);
		CObj_SetEraseColor((*color)[0], (*color)[1], (*color)[2], (*color)[3]);
		CObj_EraseScreen(cobj, 1, 1, 1);
		// LObj_DeleteCurrentAll(0);
		Match_SetStageRenderFlags(1);
		Match_SetUnkRenderFlags(1);

		gobj->cobj_links = (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7);
		CObj_RenderGXLinks(gobj, 7);

		Match_SetStageRenderFlags(0);
		Match_SetUnkRenderFlags(0);

		gobj->cobj_links = (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7);
		CObj_RenderGXLinks(gobj, 7);


		// gobj->cobj_links = (8 << 0);
		// int* links = (int*)gobj->cobj_links+4;
		// links = (int*)0x25;
		// Fog_Set(0);

		// OSReport("cobj_links: %p\n", gobj->cobj_links);
		// OSReport("val: %p\n", &gobj->cobj_links);
		// OSReport("val: %016llx\n", MainCameraGObj->cobj_links);
		// OSReport("val2: %016llx\n\n", gobj->cobj_links);

		gobj->cobj_links = (0 << 0);
		Match_SetStageRenderFlags(0);
		Match_SetUnkRenderFlags(0);
		CObj_EndCurrent();
	}

	HSD_ImageDescCopyFromEFB(img->image, 250, 200, true);
	CObj_SetCurrent(MainCameraGObj->hsd_object);
	
	CObj_SetEraseColor((*color)[0], (*color)[1], (*color)[2], (*color)[3]);
	CObj_EraseScreen(cobj, 1, 1, 1);
	MainCameraGObj->cobj_links = (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7);
	Match_SetStageRenderFlags(1);
	Match_SetUnkRenderFlags(1);
	CObj_RenderGXLinks(MainCameraGObj, 7);

	MainCameraGObj->cobj_links = (1 << 3) + (1 << 4) + (1 << 5) + (1 << 6) + (1 << 7);
	Match_SetStageRenderFlags(0);
	Match_SetUnkRenderFlags(0);
	CObj_RenderGXLinks(MainCameraGObj, 7);


	// C_MTXLightPerspective(cobj->projection_param.perspective.fov, cobj->projection_param.perspective.aspect, -0.49f, 0.49f, 0.5f, 0.5f, mtx);
	// COBJ_GetViewingMtx(cobj, b);
	// PSMTXConcat(img_mtx, b, img_mtx);
	
}