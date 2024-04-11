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

static void _SetMatAnim(JOBJ* joint, float start, bool rate) {
	float fps = 0;
	if (rate) {
		fps = 60;
	}
	// call aobj functions directly, so we play the aobjs we want
	AOBJ *tex_aobj = joint->dobj->mobj->tobj->aobj;
	if (!tex_aobj) return;
	// Sets the matanim frame
	AOBJ_SetRate(tex_aobj,fps); // rate = frame rate
	AOBJ_ReqAnim((int *)tex_aobj, start); // unk = frame
}

static void _PlayJointAnim(JOBJ *joint, float start, float end) {
	AOBJ *joint_aobj = JOBJ_GetJointAOBJ(joint);
	if (!joint_aobj) return;
	HSD_AObjSetEndFrame(end, joint_aobj);
	// JOBJ_ReqAnim(joint, start);
	AOBJ_ReqAnim((int *)joint_aobj, start);
}

static void HandleFaceState(GOBJ *gobj) {
	CardFace *fd = gobj->userdata;
	int face_idx = fd->face_index;
	JOBJ* curr_face = fd->face_jobjs[face_idx];
	CardFace_State face_state = fd->face_state[face_idx];

	if (face_state == CardFace_State_INIT) {
		_SetMatAnim(curr_face, 2, false);
		_PlayJointAnim(curr_face, 0.0, 90);
		HSD_Material *face_mat = curr_face->dobj->mobj->mat;
		face_mat->diffuse = (GXColor){255, 255, 255, 255};
		fd->face_state[fd->face_index] = CardFace_State_SELECTED;
	}
	else if (face_state == CardFace_State_DISABLED) {
		_SetMatAnim(curr_face, 2, false);
		_PlayJointAnim(curr_face, 0.0, 90);
		HSD_Material *face_mat = curr_face->dobj->mobj->mat;
		face_mat->diffuse = (GXColor){255, 255, 255, 255};
		fd->face_state[fd->face_index] = CardFace_State_SELECTED;
	}
	
	for (int i = 0; i < FACE_COUNT; i++) {
		if (face_idx != i) {
			if (fd->face_state[i] == CardFace_State_SELECTED) {
				fd->face_state[i] = CardFace_State_DISABLED;
				HSD_Material *face_mat = fd->face_jobjs[i]->dobj->mobj->mat;
  				face_mat->diffuse = (GXColor){128, 128, 128, 255};

				_PlayJointAnim(fd->face_jobjs[i], 110, 180);
				OSReport("Face: %d State: %d\n", i, fd->face_state[i]);
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
	// remove anim so it doesn't auto play
	JOBJ_RemoveAnimAll(fd->root_jobj);
	JOBJ_AddSetAnim(fd->root_jobj, fd->jobj_set, 0);

	GOBJ *gobj = GObj_Create(0x4, 0xf, 0);
	_FaceHandlerGObj = gobj;

    for (int i = FACE_ONE; i < FACE_COUNT; i++) {
        JOBJ *target_sibling = GetNthSibling(fd->root_jobj->child, i);
        fd->face_jobjs[i] = target_sibling;
		fd->face_state[i] = CardFace_State_INIT;
		_SetMatAnim(target_sibling, 3, false);
		HSD_Material *face_mat = fd->face_jobjs[i]->dobj->mobj->mat;
		face_mat->diffuse = (GXColor){128, 128, 128, 255};
    }

	GObj_AddUserData(gobj, 0, FreeFaceHandler, fd);
	GObj_AddProc(gobj, HandleFaceState, 6);

	JOBJ *hover_jobjs = GetNthSibling(fd->root_jobj->child, 4);
	JOBJ_SetFlagsAll(hover_jobjs, JOBJ_HIDDEN);

	return fd;
}
