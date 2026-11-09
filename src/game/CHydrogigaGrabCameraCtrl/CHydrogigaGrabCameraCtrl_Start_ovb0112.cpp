// CHydrogigaGrabCameraCtrl (unit 1324): the start, as CEmmyFloorSlideCameraCtrl's
// (src/game/CEmmyFloorSlideCameraCtrl/CEmmyFloorSlideCameraCtrl_b44.cpp): the vector at +0xb4 of the
// argument's definition (+0x130) and the position of its node (+0x20, fn_7100206154) go to the
// current (+0x74, +0x68) and start (+0x44, +0x38) slots, then the base step (fn_71010ed98c); a pending
// camera id (+0x1d8) goes to the game manager (fn_7100e038b0) and, while the timer at +0x1ec runs, the
// blend is reset and the shared settings at +0x1f8 are handed over with a reference (as
// CCharClassSetSlomoCustomEvent_Execute.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkFixedCamera_fc;
struct UnkArg_fc;
struct UnkVec;
void fn_71010ed98c(UnkFixedCamera_fc* self, UnkArg_fc* arg, u32 flag);
void fn_7100e038b0(UnkGameManager*, void*, UnkVec*, u64);
void fn_7100df6c78(UnkGameManager* self, f32 a, f32 b);
void fn_7100df6d3c(UnkGameManager* self, void* settings, f32 a, f32 b);

struct UnkGrabDef_ovb0112 {
    u8 pad_0x0[0xb4];
    UnkVec3f dir;
};

struct UnkGrabArg_ovb0112 {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
    UnkGrabDef_ovb0112* def;  // 0x130
};

struct UnkGrabSettings_ovb0112 {
    u8 pad_0x0[0x10];
    u8 body[0x34];
    union {
        s32 ref_count;  // 0x44
    };
};

struct UnkGrabCamera_ovb0112 {
    u8 pad_0x0[0x38];
    UnkVec3f start_pos;  // 0x38
    UnkVec3f start_dir;  // 0x44
    u8 pad_0x50[0x18];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0x158];
    CStrId camera;  // 0x1d8
    u8 field_0x1e0[0xc];
    f32 timer;  // 0x1ec
    f32 field_0x1f0;
    u8 pad_0x1f4[0x4];
    UnkGrabSettings_ovb0112* settings;  // 0x1f8
};

void fn_710086ea48(UnkGrabCamera_ovb0112* self, UnkGrabArg_ovb0112* arg) {
    self->dir = arg->def->dir;
    self->start_dir = self->dir;
    UnkVec3f pos = fn_7100206154(&arg->node);
    self->pos = pos;
    self->start_pos = pos;
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
    if (self->camera.data != &g_unk_7101d47108->empty)
        fn_7100e038b0(g_unk_7101d47310, &self->camera, reinterpret_cast<UnkVec*>(self->field_0x1e0), 0);
    if (self->timer > 0.0f) {
        fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.0f);
        UnkGameManager* m = g_unk_7101d47310;
        f32 a = self->field_0x1f0, b = self->timer;
        ++self->settings->ref_count;
        fn_7100df6d3c(m, self->settings->body, a, b);
    }
}
