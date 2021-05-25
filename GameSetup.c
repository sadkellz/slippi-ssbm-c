#include "GameSetup.h"

#include "./Components/Button.h"
#include "./Components/CharStageBoxSelector.h"
#include "./Components/CharStageIcon.h"
#include "./m-ex/MexTK/mex.h"
#include "Files.h"

static ArchiveInfo *gui_archive;
static GUI_GameSetup *gui_assets;
static GameSetup_Data *data;

void Minor_Load(void *minor_data) {
  OSReport("minor load\n");

  // Init location to store data
  data = calloc(sizeof(GameSetup_Data));
  data->process_type = GameSetup_Process_Type_STAGE_STRIKING;

  // Set up input handler. Initialize at top to make sure it runs before anything else
  GOBJ *input_handler_gobj = GObj_Create(4, 0, 128);
  GObj_AddProc(input_handler_gobj, InputsThink, 0);

  // Load file
  gui_archive = File_Load("GameSetup_gui.dat");
  // gui_archive = File_Load("GmTou1p.dat");
  gui_assets = File_GetSymbol(gui_archive, "ScGamTour_scene_data");

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
  JOBJ_LoadSet(0, gui_assets->jobjs[5], 0, 0, 3, 1, 1, GObj_Anim);

  // Load panel and frame
  JOBJ_LoadSet(0, gui_assets->jobjs[4], 0, 0, 3, 1, 1, GObj_Anim);

  // Init selectors
  InitSelectorJobjs();

  // Prepare text
  Text *txt = Text_CreateText(0, 0);
  txt->kerning = 1;
  txt->align = 0;  // align left
  txt->scale = (Vec2){0.01, 0.01};

  // int id = Text_AddSubtext(txt, -2000, 0, "OTest");
  // Text_SetScale(txt, id, 6, 6);

  // Load confirm/change buttons
  data->buttons[0] = Button_Init(gui_assets, GUI_GameSetup_JOBJ_BUTTON_OK);
  data->buttons[1] = Button_Init(gui_assets, GUI_GameSetup_JOBJ_BUTTON_REDO);
  Button_SetPos(data->buttons[0], (Vec3){-4.8, -2.5, 0});
  Button_SetPos(data->buttons[1], (Vec3){4.8, -2.5, 0});
  // Button_SetMaterial(data->buttons[0], Button_Material_Ok);
  // Button_SetMaterial(data->buttons[1], Button_Material_Redo);
  data->button_count = 2;

  // Initialize state, init steps, and prepare current step
  InitState();
  InitSteps();
  PrepareCurrentStep();
}

void InitState() {
  data->state.selector_idx = 0;  // TODO: Set differently if first or second striker
  data->state.selected_values_count = 0;
}

void InitSteps() {
  data->step_count = 5;
  data->steps = calloc(data->step_count * sizeof(GameSetup_Step));
  data->state.step_idx = 0;

  data->steps[0].player_idx = 0;
  data->steps[0].type = GameSetup_Step_Type_CHOOSE_CHAR;
  data->steps[0].char_selection = 0;
  data->steps[0].char_color_selection = 0;

  data->steps[1].player_idx = 0;
  data->steps[1].type = GameSetup_Step_Type_CHOOSE_CHAR;
  data->steps[1].char_selection = 0;
  data->steps[1].char_color_selection = 0;

  data->steps[2].player_idx = 0;
  data->steps[2].type = GameSetup_Step_Type_REMOVE_STAGE;
  data->steps[2].required_selection_count = 1;

  data->steps[3].player_idx = 0;
  data->steps[3].type = GameSetup_Step_Type_REMOVE_STAGE;
  data->steps[3].required_selection_count = 2;

  data->steps[4].player_idx = 0;
  data->steps[4].type = GameSetup_Step_Type_REMOVE_STAGE;
  data->steps[4].required_selection_count = 1;

  // Start with the first two steps completed
  CompleteCurrentStep();
  CompleteCurrentStep();
}

void InitSelectorJobjs() {
  int count = 5;
  CSIcon_Material iconMats[] = {
      CSIcon_Material_Fountain,
      CSIcon_Material_Dreamland,
      CSIcon_Material_Yoshis,
      CSIcon_Material_Pokemon,
      CSIcon_Material_Battlefield,
  };

  // Do nothing if there are no selectors
  if (count == 0 || count > MAX_SELECTORS) {
    return;
  }

  float gap = 8;
  float yPos = 5.5;
  float xPos = 0;

  // If odd number of items, the middle point should be in between two items
  if (count % 2 == 0) {
    xPos += gap / 2;
  }

  // Move to x pos for first item
  xPos -= gap * (int)(count / 2);

  // Initialize the box selectors
  for (int i = 0; i < count; i++) {
    CSBoxSelector *csbs = CSBoxSelector_Init(gui_assets);
    CSIcon_SetMaterial(csbs->icon, iconMats[i]);
    CSBoxSelector_SetPos(csbs, (Vec3){xPos, yPos, 0});

    data->selectors[i] = csbs;
    xPos += gap;
  }

  data->selector_count = count;

  // Hover the first item
  CSBoxSelector_SetHover(data->selectors[0], true);
}

void ResetButtonState() {
  for (int i = 0; i < data->button_count; i++) {
    Button_SetHover(data->buttons[i], false);
    Button_SetVisibility(data->buttons[i], false);
  }

  data->state.btn_hover_idx = 0;
}

void Minor_Think() {
  return;
}

void Minor_Exit(void *minor_data) {
  OSReport("minor exit\n");
  return;
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

static u32 frame_counter = 0;

void InputsThink(GOBJ *gobj) {
  u8 port = R13_U8(-0x5108);
  u64 scrollInputs = Pad_GetRapidHeld(port);  // long delay between initial triggers, then frequent
  u64 downInputs = Pad_GetDown(port);

  if (frame_counter == 0) {
    char prnt[50];
    sprintf(prnt, "Port: %d\n", port);
    OSReport(prnt);
  }

  GameSetup_Step *step = &data->steps[data->state.step_idx];

  if (data->state.selector_idx >= 0) {
    ////////////////////////////////
    // Adjust scroll position
    ////////////////////////////////
    if (scrollInputs & (HSD_BUTTON_RIGHT | HSD_BUTTON_DPAD_RIGHT)) {
      // Handle a right input
      data->state.selector_idx++;
      SFX_PlayCommon(2);  // Play move SFX
    } else if (scrollInputs & (HSD_BUTTON_LEFT | HSD_BUTTON_DPAD_LEFT)) {
      // Handle a left input
      data->state.selector_idx--;
      SFX_PlayCommon(2);  // Play move SFX
    }

    if (data->state.selector_idx < 0) {
      data->state.selector_idx += data->selector_count;
    } else if (data->state.selector_idx >= data->selector_count) {
      data->state.selector_idx -= data->selector_count;
    }

    ////////////////////////////////
    // Handle confirmation button press
    ////////////////////////////////
    // TODO: Handle buttons before scroll, don't scroll on the same frame a button is pressed
    if (downInputs & HSD_BUTTON_A) {
      int idx = data->state.selector_idx;
      u8 isSelected = data->selectors[idx]->state.select_state != CSBoxSelector_Select_State_NotSelected;

      if (isSelected) {
        CSBoxSelector_SetSelectState(data->selectors[idx], CSBoxSelector_Select_State_NotSelected);
        data->state.selected_values_count--;
      } else {
        CSBoxSelector_SetSelectState(data->selectors[idx], CSBoxSelector_Select_State_X);
        data->state.selected_values_count++;
      }

      // TODO: Pick better sound?
      SFX_PlayCommon(2);
    }

    // Check condition to see if selections are complete
    if (data->state.selected_values_count >= step->required_selection_count) {
      // Clear selection index
      data->state.prev_selector_idx = data->state.selector_idx;
      data->state.selector_idx = -1;

      // Display and prepare buttons
      for (int i = 0; i < data->button_count; i++) {
        Button_SetVisibility(data->buttons[i], true);
      }

      data->state.btn_hover_idx = 0;
      Button_SetHover(data->buttons[0], true);
    }
  } else {
    ////////////////////////////////
    // Adjust button hover
    ////////////////////////////////
    if (scrollInputs & (HSD_BUTTON_RIGHT | HSD_BUTTON_DPAD_RIGHT)) {
      // Handle a right input
      data->state.btn_hover_idx++;
      SFX_PlayCommon(2);  // Play move SFX
    } else if (scrollInputs & (HSD_BUTTON_LEFT | HSD_BUTTON_DPAD_LEFT)) {
      // Handle a left input
      data->state.btn_hover_idx--;
      SFX_PlayCommon(2);  // Play move SFX
    }

    if (data->state.btn_hover_idx < 0) {
      data->state.btn_hover_idx += data->button_count;
    } else if (data->state.btn_hover_idx >= data->button_count) {
      data->state.btn_hover_idx -= data->button_count;
    }

    // TODO: Handle buttons before scroll, don't scroll on the same frame a button is pressed
    if (downInputs & HSD_BUTTON_B || (downInputs & HSD_BUTTON_A && data->state.btn_hover_idx == 1)) {
      data->state.selector_idx = data->state.prev_selector_idx;
      data->state.selected_values_count = 0;
      for (int i = 0; i < data->selector_count; i++) {
        CSBoxSelector_SetSelectState(data->selectors[i], CSBoxSelector_Select_State_NotSelected);
      }

      // Hide buttons
      ResetButtonState();

      SFX_PlayCommon(2);
    } else if (downInputs & HSD_BUTTON_A && data->state.btn_hover_idx == 0) {
      SFX_PlayCommon(1);
      CompleteCurrentStep();
      PrepareCurrentStep();
    }
  }

  // Update selector position
  for (int i = 0; i < data->selector_count; i++) {
    // Set hover state. Won't do anything if already set to that state
    CSBoxSelector_SetHover(data->selectors[i], data->state.selector_idx == i);
  }

  // Update button hover position
  for (int i = 0; i < data->button_count; i++) {
    // Set hover state. Won't do anything if already set to that state
    Button_SetHover(data->buttons[i], data->state.btn_hover_idx == i);
  }

  frame_counter++;
}

void CompleteCurrentStep() {
  // Get and complete current step
  GameSetup_Step *step = &data->steps[data->state.step_idx];
  step->state = GameSetup_Step_State_COMPLETE;

  // Commit selections to selected values
  int commit_index = 0;
  for (int i = 0; i < step->required_selection_count; i++) {
    CSBoxSelector *bs = data->selectors[i];

    // If this selector is selected, indicate that stage as the one selected
    if (bs->state.select_state == CSBoxSelector_Select_State_Selected) {
      int stageId = CSIcon_ConvertMatToStage(bs->icon->state.material);
      step->stage_selections[commit_index] = stageId;
      commit_index++;
    }
  }

  // Increment step idx
  data->state.step_idx++;
  if (data->state.step_idx >= data->step_count) {
    // TODO: Complete full process and end scene
    return;
  }

  // Activate next step
  step = &data->steps[data->state.step_idx];
  step->state = GameSetup_Step_State_ACTIVE;
}

void PrepareCurrentStep() {
  // Do nothing if step outside of bounds
  if (data->state.step_idx >= data->step_count) {
    return;
  }

  // TODO: Show/Hide the selectors used for this step and adjust number

  // Disable stages that cannot be selected and move selector index
  // Pokemon is the last stage, make array large enough to fit a bool for all of them
  u8 shouldDisableMat[CSIcon_LAST_STAGE_MAT_IDX + 1] = {false};
  for (int i = 0; i < data->state.step_idx; i++) {
    GameSetup_Step *s = &data->steps[i];
    if (s->type != GameSetup_Step_Type_REMOVE_STAGE) {
      continue;
    }

    // This is a remove stage step, grab the result and mark those stages as removed
    for (int j = 0; j < s->required_selection_count; j++) {
      u8 matIdx = CSIcon_ConvertStageToMat(s->stage_selections[j]);
      shouldDisableMat[matIdx] = true;
    }
  }

  // Go through selectors and mark the proper state
  for (int i = 0; i < data->selector_count; i++) {
    CSBoxSelector *csbs = data->selectors[i];

    CSBoxSelector_Select_State newSelectState = CSBoxSelector_Select_State_NotSelected;
    if (shouldDisableMat[csbs->icon->state.material]) {
      newSelectState = CSBoxSelector_Select_State_Disabled;
    }

    CSBoxSelector_SetSelectState(csbs, newSelectState);
  }

  // Clear button state
  ResetButtonState();
}