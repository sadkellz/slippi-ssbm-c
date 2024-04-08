#include "CardDoor.h"

#include "../Files.h"
#include "../m-ex/MexTK/mex.h"
#include "../Common.h"

GOBJ *_DoorHandlerGObj = NULL;

void FreeDoorHandler(void *ptr) {
    _DoorHandlerGObj = NULL;
	if (ptr) HSD_Free(ptr);
}

static JOBJ *GetNthSibling(JOBJ *start, int n) {
    while (start != NULL && n-- > 0) {
        start = start->sibling;
    }
    return start;
}


static void _SetEndFrame(JOBJ *joint) {
	// OSReport("joint: %p\n", joint);
	// AOBJ *aobj = JOBJ_GetJointAOBJ(joint);
	AOBJ *aobj = joint->dobj->mobj->tobj->aobj;
	if (aobj != 0) {
		HSD_AObjSetEndFrame(30, aobj);
	}
}

static void _SetDisplayState(CardDoor *cd, JOBJ *door) {

    if (cd->state[DOOR_ONE] == CardDoor_State_CLOSED) {
        // JOBJ_ReqAnimAll(door, 0);
		JOBJ_ForEachAnim(door, 6, 0x400, AOBJ_ReqAnim, 1, 0);
		cd->state[DOOR_ONE] = CardDoor_State_OPEN;
    }
    else {
        return;
    }
}

void DoorStateChange(CardDoor *cd, int door_idx) {
	JOBJ* door_jobj = cd->door_jobjs[door_idx];
    JOBJ_AddSetAnim(cd->root_jobj, cd->jobj_set, 0);
	HSD_JObjWalkTree(cd->root_jobj, _SetEndFrame, 0);

    if (cd->state[door_idx] == CardDoor_State_CLOSED) {
		// JOBJ_ReqAnim(door_jobj, 0);
        // JOBJ_ReqAnimAll(door_jobj, 0);
		cd->state[door_idx] = CardDoor_State_OPEN;
    }
    else {
        return;
    }
}

static void HandleDoorState(GOBJ *gobj) {
	CardDoor *cd = gobj->userdata;
	int door_idx = cd->door_index;
	JOBJ* curr_door = cd->door_jobjs[door_idx];
	
	if (cd->state[cd->door_index] == CardDoor_State_CLOSED) {
		JOBJ_ForEachAnim(curr_door, 6, 0x400, AOBJ_ReqAnim, 1, 0);
		cd->state[cd->door_index] = CardDoor_State_OPEN;
	}
}

CardDoor *CardDoor_Init(GUI_GameSetup *gui) {
	CardDoor *cd = calloc(sizeof(CardDoor));

	// Init cd jobj
	cd->jobj_set = gui->jobjs[GUI_NewScene_JOBJ_Doors];
	cd->gobj = JOBJ_LoadSet(0, cd->jobj_set, 0, 0, 3, 1, 1, GObj_Anim);
	cd->root_jobj = cd->gobj->hsd_object;
	JOBJ_RemoveAnimAll(cd->root_jobj);

	GOBJ *gobj = GObj_Create(0x4, 0xf, 0);
	_DoorHandlerGObj = gobj;

    for (int i = DOOR_ONE; i < DOOR_COUNT; i++) {
        JOBJ *target_sibling = GetNthSibling(cd->root_jobj->child, i);
        cd->door_jobjs[i] = target_sibling;
    }

	JOBJ_AddSetAnim(cd->root_jobj, cd->jobj_set, 0);
	HSD_JObjWalkTree(cd->root_jobj, _SetEndFrame, 0);
	GObj_AddUserData(gobj, 0, FreeDoorHandler, cd);
	GObj_AddProc(gobj, HandleDoorState, 6);

	// Only open door one
	// _door_jobj = cd->door_jobjs[DOOR_ONE];
	// OSReport("Door jobj: %p\n", cd->door_jobjs[DOOR_ONE]);
	// _SetDisplayState(cd, cd->door_jobjs[DOOR_ONE]);

	return cd;
}
