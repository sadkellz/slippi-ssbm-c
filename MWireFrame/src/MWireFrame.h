#include "../../m-ex/MexTK/mex.h"

///////////////////////
//      m-ex IDs     //
///////////////////////

// #define MEX_ITEM_FIREBALL 0
// #define MEX_ITEM_CAPE 2

// #define MEX_EFFECT_FIREBALL 5000
// #define MEX_EFFECT_FIREBALL_FLAME 5001
// #define MEX_EFFECT_DSPECIAL 5002

// #define MEX_SOUND_FIREBALL_DESTROY 5025

///////////////////////
//    Vanilla IDs    //
///////////////////////

// #define VANILLA_ITEM_FIREBALL 0x30
// //#define VANILLA_ITEM_CAPE (item id is defined in Mario's special attributes)

// #define VANILLA_EFFECT_FIREBALL 0x47A
// #define VANILLA_EFFECT_FIREBALL_FLAME 0x47B
// #define VANILLA_EFFECT_DSPECIAL 0x47C

// #define VANILLA_SOUND_FIREBALL_DESTROY 180025

///////////////////////
//  Special States   //
///////////////////////

// #define STATE_SPECIALN 295
// #define STATE_SPECIALAIRN 296
// #define STATE_SPECIALSSTART 297
// #define STATE_SPECIALS 298
// #define STATE_SPECIALSAIRSTART 299
// #define STATE_SPECIALSAIRS 300
#define STATE_SPECIALHI 353
// #define STATE_SPECIALAIRHI 302
// #define STATE_SPECIALHICATCH 303
// #define STATE_SPECIALHITHROW 304
// #define STATE_SPECIALLW 305
// #define STATE_SPECIALLWEND 306
// #define STATE_SPECIALAIRLW 307
// #define STATE_SPECIALAIRLWEND 308
// #define STATE_SPECIALLWENDAIR 309
// #define STATE_SPECIALAIRLWENDAIR 310
// #define STATE_SPECIALHITHROW2 311

///////////////////////
//    Item States    //
///////////////////////


///////////////////////
//      Structs      //
///////////////////////


// from decomp https://github.com/doldecomp/melee/blob/master/src/melee/ft/chara/ftCaptain/types.h#L16
typedef struct FalconAttr { 
    /* +00 */ float specialn_stick_range_y_neg;
    /* +04 */ float specialn_stick_range_y_pos;
    /* +08 */ float specialn_angle_diff;
    /* +0C */ float specialn_vel_x;
    /* +10 */ float specialn_vel_mul;
    /* +14 */ float specials_gr_vel_x;
    /* +18 */ float specials_grav;
    /* +1C */ float specials_terminal_vel;
    /* +20 */ float specials_unk0;
    /* +24 */ float specials_unk1;
    /* +28 */ float specials_unk2;
    /* +2C */ float specials_unk3;
    /* +30 */ float specials_unk4;
    /* +34 */ float specials_unk5;
    /* +38 */ float specials_miss_landing_lag;
    /* +3C */ float specials_hit_landing_lag;
    /* +40 */ float specialhi_air_friction_mul;
    /* +44 */ float specialhi_horz_vel;
    /* +48 */ float specialhi_freefall_air_spd_mul;
    /* +4C */ float specialhi_landing_lag;
    /* +50 */ float specialhi_unk0;
    /* +54 */ float specialhi_unk1;
    /* +58 */ float specialhi_input_var;
    /* +5C */ float specialhi_unk2;
    /* +60 */ float specialhi_catch_grav;
    /* +64 */ s32 specialhi_air_var;
    /* +68 */ float x68;
    /* +6C */ u32 speciallw_unk1;
    /* +70 */ float speciallw_flame_particle_angle;
    /* +74 */ float speciallw_on_hit_spd_modifier;
    /* +78 */ s32 speciallw_unk2;
    /* +7C */ float speciallw_ground_lag_mul;
    /* +80 */ float speciallw_landing_lag_mul;
    /* +84 */ float speciallw_ground_traction;
    /* +88 */ float speciallw_air_landing_traction;
}FalconAttr;

typedef struct SpecialHiFtCmd
{
/* fp+2340 */ u16 x0;
/* fp+2342:0 */ u8 x2_b0 : 1;
/* fp+2342:1 */ u8 x2_b1 : 1;
/* fp+2342:2 */ u8 x2_b2 : 1;
/* fp+2342:3 */ u8 x2_b3 : 1;
/* fp+2342:4 */ u8 x2_b4 : 1;
/* fp+2342:5 */ u8 x2_b5 : 1;
/* fp+2342:6 */ u8 x2_b6 : 1;
/* fp+2342:7 */ u8 x2_b7 : 1;
/* fp+2343 */ u8 x3;
/* fp+2344 */ Vec2 vel;
} SpecialHiFtCmd;


///////////////////////
//     Functions     //
///////////////////////

int (*cvt_fp2unsigned)(float unk) = (void *) 0x803228c0;
int (*unk_007d050)(float unk, FighterData *fd) = (void *) 0x8007d050;
// void (*ch_CFalcon_SpecialHi_AnimationInterrupt)(GOBJ *fighter) = (void *) 0x800e4b00;
// void* (*ch_CFalcon_SpecialHi_AnimationInterrupt)(GOBJ *fighter) = (void *) 0x800e4b00;
// void* (*ch_CFalcon_SpecialHi_AnimationInterrupt)(GOBJ *fighter) = (void *) 0x800e4b00;
// void* (*ch_CFalcon_SpecialHi_AnimationInterrupt)(GOBJ *fighter) = (void *) 0x800e4b00;
// void* (*ch_CFalcon_SpecialHi_AnimationInterrupt)(GOBJ *fighter) = (void *) 0x800e4b00;


// Think
// void FireBallThink(GOBJ *);
// void MarioCapeThink(GOBJ *);

// // Special N
// void SpecialN_AnimationCallback(GOBJ *gobj);
// void SpecialN_IASACallback(GOBJ *gobj);
// void SpecialN_PhysicCallback(GOBJ *gobj);
// void SpecialN_CollisionCallback(GOBJ *gobj);

// void SpecialAirN_AnimationCallback(GOBJ *gobj);
// void SpecialAirN_IASACallback(GOBJ *gobj);
// void SpecialAirN_PhysicCallback(GOBJ *gobj);
// void SpecialAirN_CollisionCallback(GOBJ *gobj);

// // SpecialS
// void SpecialS_AnimationCallback(GOBJ *gobj);
// void SpecialS_IASACallback(GOBJ *gobj);
// void SpecialS_PhysicCallback(GOBJ *gobj);
// void SpecialS_CollisionCallback(GOBJ *gobj);

// void SpecialAirS_AnimationCallback(GOBJ *gobj);
// void SpecialAirS_IASACallback(GOBJ *gobj);
// void SpecialAirS_PhysicCallback(GOBJ *gobj);
// void SpecialAirS_CollisionCallback(GOBJ *gobj);

// Specialhi
void SpecialHi_AnimationCallback(GOBJ *gobj);
void SpecialHi_IASACallback(GOBJ *gobj);
void SpecialHi_PhysicCallback(GOBJ *gobj);
void SpecialHi_CollisionCallback(GOBJ *gobj);

// void SpecialAirHi_AnimationCallback(GOBJ *gobj);
// void SpecialAirHi_IASACallback(GOBJ *gobj);
// void SpecialAirHi_PhysicCallback(GOBJ *gobj);
// void SpecialAirHi_CollisionCallback(GOBJ *gobj);

// // SpecialLw
// void SpecialLw_AnimationCallback(GOBJ *gobj);
// void SpecialLw_IASACallback(GOBJ *gobj);
// void SpecialLw_PhysicCallback(GOBJ *gobj);
// void SpecialLw_CollisionCallback(GOBJ *gobj);

// void SpecialAirLw_AnimationCallback(GOBJ *gobj);
// void SpecialAirLw_IASACallback(GOBJ *gobj);
// void SpecialAirLw_PhysicCallback(GOBJ *gobj);
// void SpecialAirLw_CollisionCallback(GOBJ *gobj);