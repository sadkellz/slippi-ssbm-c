#include "CardDoor.h"

#include "../Files.h"
#include "../m-ex/MexTK/mex.h"
#include "../Common.h"

GOBJ *_FaceHandlerGObj = NULL;

void FreeFaceHandler(void *ptr) {
    _FaceHandlerGObj = NULL;
	if (ptr) HSD_Free(ptr);
}

static JOBJ *GetNthSibling(JOBJ *start, int n) {
    while (start != NULL && n-- > 0) {
        start = start->sibling;
    }
    return start;
}

static void _SetEndFrame(JOBJ *joint) {
	AOBJ *aobj = JOBJ_GetJointAOBJ(joint);

	if (aobj != 0) {
		HSD_AObjSetEndFrame(90, aobj);
	}
}

// static void _SetDisplayState(CardFace *fd, JOBJ *door) {

//     if (fd->state[DOOR_ONE] == CardFace_State_CLOSED) {
//         // JOBJ_ReqAnimAll(door, 0);
// 		JOBJ_ForEachAnim(door, 6, 0x400, AOBJ_ReqAnim, 1, 0);
// 		fd->state[DOOR_ONE] = CardFace_State_OPEN;
//     }
//     else {
//         return;
//     }
// }

static void HandleFaceState(GOBJ *gobj) {
	CardFace *fd = gobj->userdata;
	int face_idx = fd->face_index;
	JOBJ* curr_face = fd->face_jobjs[face_idx];
	CardFace_State face_state = fd->face_state[face_idx];

	if (face_state == CardFace_State_INIT) {
		AOBJ *aobj = JOBJ_GetJointAOBJ(curr_face);
		HSD_AObjSetEndFrame(90, aobj);
		JOBJ_ReqAnimAll(curr_face, 0);
		// JOBJ_ForEachAnim(curr_door, 6, 0x400, AOBJ_ReqAnim, 1, 0);
		fd->face_state[fd->face_index] = CardFace_State_SELECTED;
	}
	else if (face_state == CardFace_State_DISABLED) {
		AOBJ *aobj = JOBJ_GetJointAOBJ(curr_face);
		HSD_AObjSetEndFrame(90, aobj);
		JOBJ_ReqAnimAll(curr_face, 0);
		fd->face_state[fd->face_index] = CardFace_State_SELECTED;
	}
	
	for (int i = 0; i < FACE_COUNT; i++) {
		if (face_idx != i) {
			if (fd->face_state[i] == CardFace_State_SELECTED) {
				fd->face_state[i] = CardFace_State_DISABLED;
				AOBJ *aobj = JOBJ_GetJointAOBJ(fd->face_jobjs[i]);
				HSD_AObjSetEndFrame(180, aobj);
				OSReport("Face: %d State: %d\n", i, fd->face_state[i]);
				JOBJ_ReqAnimAll(fd->face_jobjs[i], 110);
			}
		}

	}
}

CardFace *CardFace_Init(GUI_GameSetup *gui) {
	CardFace *fd = calloc(sizeof(CardFace));

	// Init fd jobj
	fd->jobj_set = gui->jobjs[GUI_NewScene_JOBJ_Face];
	fd->gobj = JOBJ_LoadSet(0, fd->jobj_set, 0, 0, 3, 1, 1, GObj_Anim);
	fd->root_jobj = fd->gobj->hsd_object;
	// remove anim so it doesn't auto 
	JOBJ_RemoveAnimAll(fd->root_jobj);

	GOBJ *gobj = GObj_Create(0x4, 0xf, 0);
	_FaceHandlerGObj = gobj;

    for (int i = FACE_ONE; i < FACE_COUNT; i++) {
        JOBJ *target_sibling = GetNthSibling(fd->root_jobj->child, i);
        fd->face_jobjs[i] = target_sibling;

		Vec3 pos = fd->face_jobjs[i]->trans;
		Vec4 rot = fd->face_jobjs[i]->rot;
		fd->face_state[i] = CardFace_State_INIT;
    }

	JOBJ_AddSetAnim(fd->root_jobj, fd->jobj_set, 0);
	// HSD_JObjWalkTree(fd->root_jobj, _SetEndFrame, 0);
	GObj_AddUserData(gobj, 0, FreeFaceHandler, fd);
	GObj_AddProc(gobj, HandleFaceState, 6);

	JOBJ *hover_jobjs = GetNthSibling(fd->root_jobj->child, 4);
	JOBJ_SetFlagsAll(hover_jobjs, JOBJ_HIDDEN);

	return fd;
}
