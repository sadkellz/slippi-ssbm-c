#include "MWireFrame.h"

///////////////////////
//    Mario Basic    //
///////////////////////
///
/// 0x800E0960
///
void OnLoad(GOBJ *gobj)
{
	// get fighter data
	FighterData *fighter_data = gobj->userdata;

	// copy attributes
	memcpy(fighter_data->special_attributes2, fighter_data->ftData->ext_attr, sizeof(FalconAttr));

	// copy pointer to attributes
	fighter_data->special_attributes = fighter_data->special_attributes2;

}
///
/// 0x800E08CC
///
void OnSpawn(GOBJ *gobj)
{
	FighterData *fighter_data = gobj->userdata;

	// clear the fighter flags
	Fighter_SetCharacterFlags(gobj, 0, 0);

	// reset character variables 1-6
	fighter_data->fighter_var.ft_var1 = 0;
	fighter_data->fighter_var.ft_var2 = 0;
	return;
}
///
///
///
__attribute__((used)) static struct FtState move_logic[] = {
		// State: 341 - Animation: FFFFFFFF
	{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
	// State: 342 - Animation: FFFFFFFF
	{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
			{
		-1,	 // AnimationID
		0x0, // StateFlags
		0x1, // AttackID
		0x0, // BitFlags
		0x0, // AnimationCallback
		0x0, // IASACallback
		0x0, // PhysicsCallback
		0x0, // CollisionCallback
		0x0, // CameraCallback
	},
		// State: 347 - SpecialN
	{
		295,        // AnimationID
		0x340211,   // StateFlags
		0x12,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E2C80, // AnimationCallback
		(void *)0x800E2E38, // IASACallback
		(void *)0x800E2F2C, // PhysicsCallback
		(void *)0x800E3168, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
		// State: 341 - SpecialHi
	{
		296,        // AnimationID
		0x340013,   // StateFlags
		0x14,       // AttackID
		0x0,        // BitFlags
		(void *)0x800e4b00,// SpecialHi_AnimationCallback, // AnimationCallback
		(void *)0x800e4b60,// SpecialHi_IASACallback, // IASACallback
		(void *)0x800e4bf8,// SpecialHi_PhysicCallback, // PhysicsCallback
		(void *)0x800e4e1c,// SpecialHi_CollisionCallback, // CollisionCallback
		Fighter_UpdateCameraBox, // CameraCallback
	}
}; 
