// CEmmyFloorSlideCameraCtrl (tools/tu_map.py; the name from reflection): the constructor (the base
// constructor fn_71010ed044, the vtable, the slide slots cleared, 0.3 at +0x1f8) and the start. Two
// state members are cleared, the position of the argument's node (+0x20, fn_7100206154) and the
// vector at +0xb4 of its definition (+0x130) go to the current (+0x68, +0x74), start (+0x38, +0x44)
// and slide (+0x1dc, +0x1e8) slots, then the base step runs (fn_71010ed98c, as in
// src/game/CFixedCameraCtrl/CFixedCameraCtrl_b40.cpp). Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkBase_7100763ed4;
void fn_71010ed044(UnkBase_7100763ed4*);
extern void* g_unk_7101a72188[];  // the class's vtable

struct UnkFixedCamera_fc;
struct UnkArg_fc;
void fn_71010ed98c(UnkFixedCamera_fc* self, UnkArg_fc* arg, u32 flag);

struct UnkSlideDef_b44a2 {
    u8 pad_0x0[0xb4];
    UnkVec3f dir;
};

struct UnkSlideArg_b44a2 {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
    UnkSlideDef_b44a2* def;  // 0x130
};

struct UnkSlideCamera_b44a2 {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x9];
    bool field_0x35;
    u8 pad_0x36[0x2];
    UnkVec3f start_pos;  // 0x38
    UnkVec3f start_dir;  // 0x44
    u8 pad_0x50[0x18];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0x158];
    bool field_0x1d8;
    u8 pad_0x1d9[3];
    UnkVec3f slide_pos;  // 0x1dc
    UnkVec3f slide_dir;  // 0x1e8
    u32 field_0x1f4;
    f32 field_0x1f8;
};

void fn_71007d324c(UnkSlideCamera_b44a2* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->field_0x1d8 = false;
    self->vtable = g_unk_7101a72188 + 2;
    __builtin_memset(&self->slide_pos, 0, 0x18);
    self->field_0x28 = 3;
    self->field_0x35 = true;
    self->field_0x1f4 = 0;
    self->field_0x1f8 = 0.3f;
}

void fn_71007d32d4(UnkSlideCamera_b44a2* self, UnkSlideArg_b44a2* arg) {
    self->field_0x1d8 = false;
    self->field_0x1f4 = 0;
    UnkVec3f pos = fn_7100206154(&arg->node);
    self->pos = pos;
    self->start_pos = pos;
    self->slide_pos = pos;
    self->dir = arg->def->dir;
    self->start_dir = self->dir;
    self->slide_dir = self->dir;
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
}
