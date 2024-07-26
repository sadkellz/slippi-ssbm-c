#include "../../m-ex/MexTK/mex.h"
#include "../../Components/Button.h"
#include "../../Components/CharPickerDialog.h"
#include "../../Components/CharStageBoxSelector.h"
#include "../../Components/FlatTexture.h"
#include "../../Components/GameResult.h"
#include "../../Components/RightArrow.h"
#include "../../Components/TurnIndicator.h"
#include "../../ExiSlippi.h"
#include "../../../Common.h"

#include "../../Components/CardDoor.h"


// How to auto-format code: https://stackoverflow.com/a/50489812/1249024

#define STRIKE_STAGE_SELECTOR_COUNT 5
#define CP_STAGE_SELECTOR_COUNT 6

#define GRACE_SECONDS 3
#define WARN_SECONDS 10
#define PANIC_SECONDS 3
#define WAIT_TIMEOUT_SECONDS 15

// This struct is defined in asm via directives and has no padding, so we need to use pack(1)
#pragma pack(1)
typedef struct GameSetup_SceneData {
  u8 max_games;
  u16 cur_game;
  u8 score_by_player[2];
  u8 prev_winner;
  u8 tiebreak_game_num;
  u8 game_results[9];
  u16 last_stage_win_by_player[2];
  u8 color_ban_active;
  u8 color_ban_char;
  u8 color_ban_color;
  u8 last_game_end_mode;
  u8 (*get_winner_idx)();
} GameSetup_SceneData;
#pragma pack()


typedef struct TurnsSetup_Data {
  CardDoor *cd;
  CardFace *fd;
} TurnsSetup_Data;


typedef struct SharedMinorData
{
  GOBJ *cam_gobj;
  COBJ *cam_cobj;
  GOBJ *player;
  Vec3 *player_pos;
  Vec3 *cam_pos;
} SharedMinorData;


typedef struct CostumeModelInfo {
  JOBJDesc *desc;
  void *matanim_data;
  u8 unk0;
  u8 unk1;
  u8 unk2;
  u8 unk3;
  u8 unk4;
  u8 unk5;
  u8 unk6;
  u8 unk7;
  u8 unk8;
  u8 unk9;
  u8 unk10;
  u8 unk11;
  void *joint_data;
} CostumeModelInfo;

typedef struct CharacterModelInfo {
  CostumeModelInfo** costumes;
  u8 costume_count;
  u8 unk1;
  u8 unk2;
  u8 unk3
} CharacterModelInfo;

typedef struct PlayerInitData {
  u8 character_kind;
  u8 slot_type;
  u8 stocks;
  u8 color;
  u8 port;
  u8 spawnpoint;
  u8 spawn_direction;
  u8 subcolor;
  u8 handicap;
  u8 team;
  u8 nametag;
  u8 x0b;
  u8 x0c;
  u8 x0d;
  u8 x0e;
  u8 cpu_level;
  u8 x10;
  u8 x11;
  short x12;
  u8 x14;
  u8 x15;
  u8 x16;
  u8 x17;
  float offense_ratio;
  float defense_ratio;
  u8 x20;
  u8 x21;
  u8 x22;
  u8 x23;
} PlayerInitData;

typedef struct PlayerCreateArgs {
  FighterKind character_id;
  u8 slot;
  u8 unk_idx;
  u8 flags_b0 : 1;
  u8 flags_b1 : 1;
} PlayerCreateArgs;

typedef struct DynamicModelDesc {
  JOBJ * joint;
  void ** anim_joint;
  void ** matanims;
  void ** shapeanims;
} DynamicModelDesc;

typedef struct SceneDesc {
  GUI_GameSetup ** models;
  COBJDesc ** cameras;
  LObjDesc ** lights;
  HSD_FogDesc ** fogs;
} SceneDesc;

typedef struct PIPData {
  Mtx* texture_mtx;
  _HSD_ImageDesc *image;
} PIPData;


typedef struct StartMeleeData StartMeleeData, *PStartMeleeData;
typedef struct StartMeleeRules StartMeleeRules, *PStartMeleeRules;
typedef struct PlayerInitData PlayerInitData, *PPlayerInitData;

CharacterKind (*VictoryScreen_GetResultScreenAnimation)(CharacterKind kind) = (int *) 0x80160438;
CharacterKind (*LoadCharacterJObjDesc)(CharacterKind kind, int costume) = (int *) 0x80085820;
CharacterKind (*VictoryScreen_InitAnimSymbols)(CharacterKind kind, HSD_Archive result, int anim) = (void *) 0x80036e20;
void* (*PlayerBlock_SetCharacterKind)(int slot, int kind) = (void *) 0x800323a0;
void* (*PlayerBlock_SetColor)(int slot, int costume_id) = (void *) 0x80033208;
void* (*PlayerBlock_SetPort)(int slot, int slot2) = (void *) 0x800334cc;
void* (*PlayerBlock_SetSlotType)(int slot, int type) = (void *) 0x8003254c;
void (*EnterVictoryAnimation)(uint slot, int pose) = (void *) 0x80036f34;
void* (*PlayerBlock_SetSpawnDirection)(double dir, int slot) = (void *) 0x80033094;
void* (*PlayerBlock_StoreModelScale)(double scale, int slot) = (void *) 0x80033b3c;
void* (*InitializePlayerInitData)(PlayerInitData *init_data) = 0x8016795c;
void* (*Match_SetupPlayer)(int slot, PlayerInitData *init_data) = 0x8016d8ac;
void* (*SetupPlayerSlot)(int slot) = (void *) 0x80031ad0;
GOBJ* (*Player_Create)(PlayerCreateArgs *args) = 0x80068e98;
void* (*SetupPlayerClass)(void) = (void *) 0x800679b0;
void* (*Player_InitFigaTreeAllocData)(void) = (void *) 0x80067a84;
void* (*CameraInfo_Init)(int pool_size) = (void *) 0x80028b9c;
void* (*FtJObjInfoInit)(void) = (void *) 0x80073700;
void* (*VictoryScreen_Think)(GOBJ *gobj) = (void *) 0x80179350;
void* (*Item_GlobalInit)(void) = (void *) 0x80266fcc;
void* (*HSD_CObjGetEyePosition)(COBJ *cobj,Vec3 *pos) = (void *) 0x80368784;
GOBJ* (*Match_SetupPlayerVictoryPose)(CharacterKind kind,int color,uint slot) = (GOBJ *) 0x8017a67c;
void* (*GObj_SetJObjVisible)(GOBJ *gobj) = (void *) 0x8026bb20;
void* (*Match_InitPlayers)(void) = (void *) 0x8016e2bc;
void* (*InitializeStartMeleeData)(MatchInit *data) = (void *) 0x80167a64;
void* (*StartMelee)(MatchInit *data) = (void *) 0x8016e730;
void* (*VsMode_Enter)(MatchInit *data) = (void *) 0x8016e934;
void* (*LoadCharacterFigaTree)(CharacterKind kind) = (void *) 0x80085a14;
void* (*Stages_Start_Unk)(int id) = (void *) 0x802251e8;
void* (*Stage_CallSetup)(void) = (void *) 0x8022524c;
void* (*RefractInit)(void) = (void *) 0x800222a4;

bool* (*Archive_CheckPreloadAndGetSymbols)(HSD_Archive **out, char *path,...) = (void *) 0x80017040;

void* (*StageUnk_002785c)(void) = (void *) 0x8002785c;
void* (*StageUnk_ScrollZ)(void) = (void *) 0x801c20d0;
void* (*StageUnk_DoInfoTable)(void) = (void *) 0x80225298;
void* (*Stage_Fountain_Init)(void) = (void *) 0x801cbb88;
GOBJ* (*Stage_Fountain_CreateGObj)(int fod) = (void *) 0x801cbce8;
void* (*Stage_Fountain_SetupModel)(GOBJ *gobj) = (void *) 0x801cbe64;


void* (*TrainingMode_Init)(MatchInit *data) = (void *) 0x8016ec28;
void* (*VictoryScreen_Init)(MatchInit *data) = (void *) 0x80177368;
void* (*VictoryScreen_Init2)(MatchInit *data) = (void *) 0x8017aa78;


// COBJ FUNCTIONS
void* (*CObjGetEyePosition)(COBJ *cobj, Vec3 *pos) = (void*) 0x80368784;
void* (*CObjGetInterest)(COBJ *cobj, Vec3 *pos) = (void*) 0x803686ac;
void* (*CObjGetEyeVector)(COBJ *cobj, Vec3 *out) = (void*) 0x8036885c;
float* (*CObjGetEyeDistance)(COBJ *cobj) = (void*) 0x80368a08;
float* (*CObjGetLeftVector)(COBJ *cobj, Vec3 *out) = (void*) 0x803692e8;
float* (*CObjGetUpVector)(COBJ *cobj, Vec3 *out) = (void*) 0x80368e70;
void* (*CObj_UpdateFromCamera)(GOBJ *gobj) = (void*) 0x8002a4ac;
void* (*CopyCObjFromMainCamera)(GOBJ *gobj) = (void*) 0x8021eb10;
void* (*Match_SetUnkRenderFlags)(int flags) = (void*) 0x80031074;
void* (*Camera_SetFixed)(void) = (void*) 0x8002f8f4;
void* (*UpdateReflection)(GOBJ *gobj) = (void*) 0x801ccea0;
void* (*C_MTXPerspective)(float fov, float aspect, float near, float far, Mtx *mtx) = (void*) 0x80342bec;
void* (*C_MTXLightPerspective)(float fov, float aspect, float stretch_x, float stretch_y, float z_x_mult, float z_y_mult, Mtx *out) = (void*) 0x80342954;




// GX FUNCTIONS
#define GX_FIFO 0xcc008000
void* (*DrawQuad)(Vec3 *corner1, Vec3 *corner2, GXColor *color) = (void*) 0x80009dd4;
void* (*Setup2DDrawing)( float scale_x, float scale_y, int line_width) = (void*) 0x80391a04;
void* (*DrawStagePoints)( short *type, int param, u8 *color) = (void*) 0x80059fe0;
void* (*SetDrawColor)(u8 *color) = (void*) 0x80058acc;
void* (*HSD_LObjSetupInit)(COBJ *cobj) = (void*) 0x80365f28;
void* (*StateSetPointSize)(byte param_1,int param_2) = (void*) 0x80361c60;
void* (*VictoryScreen_RenderTargetCam)(int slot) = (void*) 0x8017a318;



// DEBUG FUNCTIONS
void* (*DevelopCam_UpdateTransforms)(GOBJ *text, int port) = (void*) 0x80227904;
void* (*DevelopCam_OrbitCam)(float cstick_x, float cstick_y, GOBJ *CameraGObj,Vec3 *CameraPosition, Vec3 *CameraInterest) = (void*) 0x80227b64;

GOBJ* (*Camera_LoadCameraEntity)(void) = (void*) 0x80030a50;
void* (*CObjSetInterest)(COBJ *cobj, Vec3 *pos) = (void*) 0x80368718;

void* (*DevelopMode_SceneUpdate)(int *cb) = (void*) 0x801a4970;
byte* (*GetCurrentRenderPass)(void) = (void*) 0x80375530;

// MATH FUNCTIONS
float (*tan)(float x) = (void*) 0x803261bc;
float (*tanf)(float x) = (void*) 0x8000cee0;
void* (*HSD_MtxInverse)(Mtx *in, Mtx *inverse) = (void*) 0x80379310;
void* (*VectorRotate)(Vec3 *v, int axis, float *angle) = (void*) 0x8000db00;


CharacterModelInfo* (MODEL_INFO) = 0x803c0ec0;
MatchInit* (VsModeEnterData) = 0x80480530;

void load_kirby();
void HandleCardInputs();
int CountTeam();
void store_except(int except[], int slot);

void extract_frustum_near(float fov, float aspect, float near, float far, Vec3 *position, Vec3 *forward, Vec3 *up, Vec3* vertices);
void drawCubeWithNormals();
void extract_frustum_far(float fov, float aspect, float near, float far, Vec3 *position, Vec3 *forward, Vec3 *up, Vec3* vertices);
GOBJ* CreateCamera();
void UpdateRenderTarget(GOBJ* gobj);

void ListPreloadFighters();
void CObjThink(GOBJ *gobj);
void InputsThink(GOBJ *gobj);
void drawing_think(GOBJ *gobj);
void create_drawing(GOBJ *gobj);
void HandleOpponentStep();
void InitAllSelectorJobjs();
void InitSelectorJobjs(CSIcon_Material *iconMats, CSBoxSelector **selectors, int count);
void InitState();
void InitStrikingSteps();
void InitCounterpickingSteps();
void InitSteps();
void InitHeader();
void ResetButtonState(u8 is_visible);
void CompleteCurrentStep(int committed_count);
void SetMatchSelections(u8 char_id, u8 char_color, u8 char_option, u16 stage_id, u8 stage_option);
void PrepareCurrentStep();
void CompleteGamePrep();
void ShowDisconnectedMessage();

void OnCharSelectionComplete(CharPickerDialog *cpd, u8 is_selection);
u8 GetNextColor(u8 charId, u8 colorId, int incr);
u8 IsColorAllowed(u8 charId, u8 colorId, u8 playerIdx);

void IncrementSelectorIndex();
void DecrementSelectorIndex();
void ResetSelectorIndex();

u8 UpdateTimer();  // Returns bool indicating if time is elapsed
void UpdateHoverDisplays();
void UpdateTimeline();