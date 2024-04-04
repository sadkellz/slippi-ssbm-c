#include "NewScene.h"
#include "../../Components/Button.h"
#include "../../Components/CharStageBoxSelector.h"
#include "../../Components/CharStageIcon.h"
#include "../../Components/StockIcon.h"
#include "../../Game/Characters.h"
#include "../../Game/Sounds.h"
#include "../../Files.h"

static HSD_Archive *gui_archive;
static GUI_GameSetup *gui_assets;
// static GameSetup_Data *data;
SharedMinorData sharedData;
GOBJ* playerObject = NULL;
Text* text;
static Vec3 staticCamPos = {0.0f, 500.0f, 0.0f};
void minor_load() {
OSReport("New Scene minor load\n");

  // Set up input handler. Initialize at top to make sure it runs before anything else
  // GOBJ *input_handler_gobj = GObj_Create(4, 0, 128);
  // GObj_AddProc(input_handler_gobj, InputsThink, 0);

  // Load file
  gui_archive = Archive_LoadFile("CustomSceneGUI.dat");
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

  sharedData.cam_gobj = cam_gobj;
  sharedData.cam_cobj = cam_cobj;

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
  // JOBJ_LoadSet(0, gui_assets->jobjs[GUI_GameSetup_JOBJ_Background], 0, 0, 3, 1, 1, GObj_Anim);

  // Load panel and frame
  // JOBJ_LoadSet(0, gui_assets->jobjs[GUI_GameSetup_JOBJ_Panels], 0, 0, 3, 1, 1, GObj_Anim);


//////////////////////////////////
Item_GlobalInit();
Player_InitFigaTreeAllocData();
Preload* preload = Preload_GetTable();
OSReport("Address of Preload Table: %p\n", (void*)preload);

uint slot = 0;
CharacterKind character = CKIND_FOX;
FighterKind fighter = FTKIND_FOX;
u8 costume = 0;

PlayerCreateArgs args;
args.character_id = fighter;
args.slot = 0;
args.unk_idx = 0xff;
args.flags2 = 0x2a;
args.unk1 = 0xb0;

PlayerBlock_StoreModelScale(1, 0);
// LoadCharacterJObjDesc(character, costume);
SetupPlayerClass();
CameraInfo_Init(8);

preload->queued.fighters[0].kind = character;
preload->queued.fighters[0].costume = costume;
Preload_Update();
// FtJObjInfoInit();
// GOBJ* playerObject = Player_Create(&args);
playerObject = Match_SetupPlayerVictoryPose(character, 0, slot);
sharedData.player = playerObject;

// FighterData *fighter_data = playerObject->userdata;
// Fighter_InitCameraBox(fighter_data);
// Vec3 setPos = {0, 0, 0};
// Fighter_SetPosition(0, 0,&setPos);
  // Vec3 currentPos = {-1.0, -1.0, -1.0};
  // Fighter_GetPosition(0, &currentPos);
  // GOBJ *GObj = Fighter_GetGObj(0);
  GOBJ *GObj = playerObject;
  OSReport("GObj during load: %p\n", GObj);
  
  BGM_Play(10);

  text = Text_CreateText(0, 0);
	text->kerning = 1;
	text->align = 0;
	text->trans.Z = 0.0f;
	text->scale.X = 0.2f;
	text->scale.Y = 0.2f;
  Text_AddSubtext(text, -125.0f, 5.0f, "james hudson");
  
  HSD_CObjGetEyePosition(cam_cobj, sharedData.cam_pos);
  OSReport("Cam pos: x = %f, y = %f, z = %f\n", sharedData.cam_pos->X, sharedData.cam_pos->Y, sharedData.cam_pos->Z);
  sharedData.cam_pos = &staticCamPos;
  CObj_SetEyePosition(cam_cobj, sharedData.cam_pos);
  HSD_CObjGetEyePosition(cam_cobj, sharedData.cam_pos);
  OSReport("Cam pos new: x = %f, y = %f, z = %f\n", sharedData.cam_pos->X, sharedData.cam_pos->Y, sharedData.cam_pos->Z);

}

void minor_think() {
  HSD_Pad* pad = PadGet(0, PADGET_ENGINE);

  if(pad->down & PAD_BUTTON_A) {
    GOBJ *player = sharedData.player;
    OSReport("GObj during think: %p\n", player);
  }

  if(pad->down & PAD_BUTTON_X) {
    Vec3 *current_pos = sharedData.player_pos;
    GOBJ *player = sharedData.player;
    Fighter_GetECBPosition(player, current_pos);
    OSReport("Current ECB Pos: x = %f, y = %f, z = %f\n", current_pos->X, current_pos->Y, current_pos->Z);
  }

  if(pad->down & PAD_BUTTON_B) {
  Scene_ExitMinor();
  }

  // return;
}

void minor_exit() {
  OSReport("New Scene minor exit\n");
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
