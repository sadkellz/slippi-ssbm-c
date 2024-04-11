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
	// State: 348 - SpecialAirN
	{
		296,        // AnimationID
		0x340611,   // StateFlags
		0x12,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E2D5C, // AnimationCallback
		(void *)0x800E2E3C, // IASACallback
		(void *)0x800E3018, // PhysicsCallback
		(void *)0x800E31F4, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
		// State: 349 - SpecialSStart
	{
		297,        // AnimationID
		0x340212,   // StateFlags
		0x13,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E384C, // AnimationCallback
		(void *)0x800E3B0C, // IASACallback
		(void *)0x800E3B1C, // PhysicsCallback
		(void *)0x800E3C38, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
		// State: 350 - SpecialS
	{
		298,        // AnimationID
		0x340212,   // StateFlags
		0x13,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E3888, // AnimationCallback
		(void *)0x800E3B10, // IASACallback
		(void *)0x800E3B3C, // PhysicsCallback
		(void *)0x800E3D44, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 351 - SpecialAirSStart
	{
		299,        // AnimationID
		0x340612,   // StateFlags
		0x13,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E3964, // AnimationCallback
		(void *)0x800E3B14, // IASACallback
		(void *)0x800E3B5C, // PhysicsCallback
		(void *)0x800E3DE4, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 352 - SpecialAirS
	{
		300,        // AnimationID
		0x340612,   // StateFlags
		0x13,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E39F0, // AnimationCallback
		(void *)0x800E3B18, // IASACallback
		(void *)0x800E3BD0, // PhysicsCallback
		(void *)0x800E3E40, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 353 - SpecialHi
	{
		301,        // AnimationID
		0x340013,   // StateFlags
		0x14,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E4B00, // AnimationCallback
		(void *)0x800E4B60, // IASACallback
		(void *)0x800E4BF8, // PhysicsCallback
		(void *)0x800E4E1C, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 354 - SpecialAirHi
	{
		302,        // AnimationID
		0x340413,   // StateFlags
		0x14,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E4EE8, // AnimationCallback
		(void *)0x800E4F48, // IASACallback
		(void *)0x800E4FDC, // PhysicsCallback
		(void *)0x800E50D8, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 355 - SpecialHiCatch
	{
		303,        // AnimationID
		0x340013,   // StateFlags
		0x14,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E51F8, // AnimationCallback
		(void *)0x800E5234, // IASACallback
		(void *)0x800E5238, // PhysicsCallback
		(void *)0x800E523C, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 356 - SpecialHiThrow
	{
		304,        // AnimationID
		0x340013,   // StateFlags
		0x14,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E5310, // AnimationCallback
		(void *)0x800E5384, // IASACallback
		(void *)0x800E5388, // PhysicsCallback
		(void *)0x800E54B8, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 357 - SpecialLw
	{
		305,        // AnimationID
		0x340214,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E415C, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E4408, // PhysicsCallback
		(void *)0x800E46B8, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 358 - SpecialLwEnd
	{
		306,        // AnimationID
		0x340214,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E4268, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E449C, // PhysicsCallback
		(void *)0x800E47B0, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 359 - SpecialAirLw
	{
		307,        // AnimationID
		0x340614,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E42E0, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E45E4, // PhysicsCallback
		(void *)0x800E48BC, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 360 - SpecialAirLwEnd
	{
		308,        // AnimationID
		0x340614,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E4354, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E4618, // PhysicsCallback
		(void *)0x800E493C, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 361 - SpecialAirLwEndAir
	{
		309,        // AnimationID
		0x340614,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E4390, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E4678, // PhysicsCallback
		(void *)0x800E495C, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 362 - SpecialLwEndAir
	{
		310,        // AnimationID
		0x340214,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E42A4, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E455C, // PhysicsCallback
		(void *)0x800E4838, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},
	// State: 363 - SpecialHiThrow
	{
		311,        // AnimationID
		0x340614,   // StateFlags
		0x15,       // AttackID
		0x0,        // BitFlags
		(void *)0x800E43CC, // AnimationCallback
		0x0,        // IASACallback
		(void *)0x800E4698, // PhysicsCallback
		(void *)0x800E49DC, // CollisionCallback
		(void *)0x800761C8, // CameraCallback
	},

}; 
