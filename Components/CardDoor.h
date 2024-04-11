#ifndef CARDDOOR_H
#define CARDDOOR_H

#include "../Files.h"
#include "../m-ex/MexTK/mex.h"
#include "../Common.h"

typedef enum CardDoor_State {
  CardDoor_State_CLOSED,
  CardDoor_State_OPEN
} CardDoor_State;

typedef enum CardDoor_Door {
  DOOR_ONE,
  DOOR_TWO,
  DOOR_THREE,
  DOOR_FOUR,
  DOOR_COUNT,
} CardDoor_Door;

typedef struct CardDoor {
  GOBJ *gobj;
  JOBJ *root_jobj;
  JOBJSet *jobj_set;
  JOBJ *door_jobjs[4];
  int door_index;
  CardDoor_State state[4];
} CardDoor;

typedef enum CardFace_Face {
  FACE_ONE,
  FACE_TWO,
  FACE_THREE,
  FACE_FOUR,
  FACE_COUNT,
} CardDoor_Face;

typedef enum CardFace_State {
  CardFace_State_INIT,
  CardFace_State_SELECTED,
  CardFace_State_DISABLED
} CardFace_State;

typedef struct CardFace {
  GOBJ *gobj;
  JOBJ *root_jobj;
  JOBJSet *jobj_set;
  JOBJ *face_jobjs[4];
  int face_index;
  CardFace_State face_state[4];
} CardFace;

void StopDoorAnim(GOBJ *proc, JOBJ *joint, int frame);
void DoorStateChange(CardDoor *cd, int door_idx);
// void _SetDisplayState(CardDoor *cd, JOBJ *door);

CardDoor *CardDoor_Init(GUI_GameSetup *gui);
CardFace *CardFace_Init(GUI_GameSetup *gui);

void* (*HSD_AObjSetEndFrame)(float frame, AOBJ *aobj) = (void *) 0x8036532c;
void* (*HSD_TObjAnimAll)(TOBJ *tobj) = (void *) 0x8035eba0;
void* (*HSD_DObjReqAnimAll)(float unk, DOBJ *dobj) = (void *) 0x8035dfd8;
void* (*HSD_JObjWalkTree)(JOBJ *jobj, void *cb, void *args) = (void *) 0x8036f0f0;


#endif