#include "MWireFrame.h"

// static void _ch_Falcon_SpecialHi_cb(GOBJ *gobj) {
// 	OSReport("callback\n");
// 	FighterData *fighter_data = gobj->userdata;
// 	FalconAttr *flAttr = fighter_data->special_attributes;
// 	SpecialHiFtCmd *script_flags = &fighter_data->ftcmd_var;


// 	fighter_data->jump.jumps_used = fighter_data->attr.max_jumps;
// 	script_flags->x0 = flAttr->specialhi_air_var;
// 	// script_flags->x0 = flAttr->specialhi_air_var;

// 	fighter_data->ftcmd_var.flag0 = 0;
// 	fighter_data->ftcmd_var.flag1 = flAttr->specialhi_unk2;

// 	script_flags->vel.X = 0.0f;
// 	script_flags->vel.Y = 0.0f;
// 	script_flags->x2_b0 = false;
// 	script_flags->x2_b1 = false;
// 	return;
// }

static void _ch_Falcon_SpecialHi_cb(GOBJ *gobj) {
	FighterData *fighter_data = gobj->userdata;
	FalconAttr *flAttr = fighter_data->special_attributes;

	SpecialHiFtCmd *as_data = &fighter_data->state_var.state_var1;
	s32 specialhi_air_var = flAttr->specialhi_air_var;
	float specialhi_unk2 = flAttr->specialhi_unk2;

	fighter_data->jump.jumps_used = (char)fighter_data->attr.max_jumps;
	as_data->x0 = (u16)specialhi_air_var;

	fighter_data->ftcmd_var.flag0 = 0;

	int unk = cvt_fp2unsigned(specialhi_unk2);
	fighter_data->ftcmd_var.flag1 = unk;

	as_data->vel.X = 0.0;
	as_data->vel.Y = 0.0;
	as_data->x2_b6 = 0;
	as_data->x2_b7 = 0;

	return;
}

////////////////////////
//  Inital SpecialHi  //
////////////////////////
///
/// 0x800e4cf4
///
void SpecialHi(GOBJ *gobj)
{
	OSReport("SpecialHi\n");
	FighterData *fighter_data = gobj->userdata;
	SpecialHiFtCmd *script_flags = &fighter_data->ftcmd_var;

	fighter_data->cb.OnStateChange = (void* ) 0x800e49fc;

	// change state and update subaction
	ActionStateChange(0, 1, 0, gobj, STATE_SPECIALHI, 0, 0);
	Fighter_InitGrab(fighter_data, 2, (void* ) 0x800e5128, 0, (void *) 0x8009ca0c);
	Fighter_AdvanceScript(gobj);

	return;
}

////////////////////////
// Grounded SpecialHi //
////////////////////////
///
/// 0x800e4b00
///

void SpecialHi_AnimationCallback(GOBJ *gobj)
{
	OSReport("SpecialHi_AnimationCallback\n");
	FighterData *fighter_data = gobj->userdata;
	FalconAttr *flAttr = fighter_data->special_attributes;

	// float specialhi_freefall_air_spd_mul = flAttr->specialhi_freefall_air_spd_mul;


	if (FrameTimerCheck(gobj) == 0)
	{
		Fighter_EnterSpecialFall(gobj, 1, 1, false, flAttr->specialhi_freefall_air_spd_mul, flAttr->specialhi_landing_lag);
	}
	return;
}
// ///
// /// 0x800e4b60
// ///
void SpecialHi_IASACallback(GOBJ *gobj)
{
	OSReport("SpecialHi_IASACallback\n");
	FighterData *fighter_data = gobj->userdata;
	FalconAttr *flAttr = fighter_data->special_attributes;
	SpecialHiFtCmd *script_flags = &fighter_data->ftcmd_var;
	
	return;
}
// ///
// /// 0x800e4bf8
// ///
void SpecialHi_PhysicCallback(GOBJ *gobj)
{
	OSReport("SpecialHi_PhysicCallback\n");
	FighterData *fighter_data = gobj->userdata;
	FalconAttr *flAttr = fighter_data->special_attributes;

	float horizontal_vel = flAttr->specialhi_horz_vel;
	float air_friction_mul = flAttr->specialhi_air_friction_mul;

	float aerial_friction = fighter_data->attr.aerial_friction;
	float aerial_drift_max = fighter_data->attr.aerial_drift_max;
	float aerial_drift_stick_mult = fighter_data->attr.aerial_drift_stick_mult;

	float x_drift_deadzone = (*stc_ftcommon)->x258;

	fighter_data->phys.self_vel.X = (float)fighter_data->state_var.state_var2;
	fighter_data->phys.self_vel.Y = (float)fighter_data->state_var.state_var3;
	fighter_data->phys.self_vel.Z = 0.0;

	int unk = unk_007d050(horizontal_vel * aerial_friction, fighter_data);

	if (unk == 0)
	{
		Fighter_AllowXDrift(fighter_data, x_drift_deadzone, aerial_drift_stick_mult * air_friction_mul, aerial_friction * horizontal_vel);
	}

	fighter_data->state_var.state_var2 = fighter_data->phys.anim_vel.X + fighter_data->phys.self_vel.X;
	fighter_data->state_var.state_var2 = fighter_data->phys.anim_vel.Y + fighter_data->phys.self_vel.Y;
	Fighter_Phys_UseAnimPos(gobj);

	fighter_data->phys.anim_vel.X = 0.0;
	fighter_data->phys.anim_vel.Y = 0.0;

	fighter_data->phys.self_vel.X = fighter_data->phys.self_vel.X + (float)fighter_data->state_var.state_var2;
	fighter_data->phys.self_vel.Y = fighter_data->phys.self_vel.Y + (float)fighter_data->state_var.state_var3;
	return;
}

// ///
// /// 0x800e1f40
// ///
void SpecialHi_OnLand(GOBJ *gobj)
{
	OSReport("SpecialHi_OnLand\n");
	FighterData *fighter_data = gobj->userdata;

	FalconAttr *flAttr = fighter_data->special_attributes;

	Fighter_EnterSpecialLanding(gobj, 0, flAttr->specialhi_landing_lag);

	return;
}
// ///
// /// 0x800E1F70
// ///
void SpecialHi_CollisionCallback(GOBJ *gobj)
{
	OSReport("SpecialHi_CollisionCallback\n");
	FighterData *fighter_data = gobj->userdata;
	SpecialHiFtCmd *script_flags = &fighter_data->ftcmd_var;
	return;
}
