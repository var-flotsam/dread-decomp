// CMetroidCameraCtrl unit (tools/tu_map.py unit 1424). fn_7100935d40: a reset: state 3 at +0x304, the time
// (fn_7100080eac of the clock g_unk_7101d47328) at +0x308, both vectors at +0x2b8 and +0x2c4 set to (0, 1e-5, 0).
// fn_710093484c: the start: the distance target (+0x254, +0x258) is the requested distance (+0x3c0) or 2250, the
// base start (fn_71010ed98c) runs, then the distance is the requested one or the tuning's (+0xb8, its +0x10, its
// +0x40, else 1665.8074); +0x250/+0x254 take +0x40, or the node's z (fn_7100206154 of the argument's +0x20) under
// 3000; after vf_0x130 the vector at +0x2c4 is copied to +0x2b8, whose x is cleared (and +0x25c set to 1) when the
// node's z is under 500. fn_7100935d94: the update: steps +0x310 (fn_710002fb80), moves the value at +0x3c4 towards
// zero at 100 per second, runs the base update (fn_7100763f9c), counts +0x300 down (or copies the current vectors
// at +0x68/+0x74 to the start ones at +0x38/+0x44 when +0x28d is set) and, while +0x304 counts, re-runs
// fn_71007642d4 once a second. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGfxFrameSource;
extern UnkGfxFrameSource* g_unk_7101d47328;
f32 fn_7100080eac(UnkGfxFrameSource* clock);
struct UnkFixedCamera_fc;
struct UnkArg_fc;
void fn_71010ed98c(UnkFixedCamera_fc* self, UnkArg_fc* arg, u32 flag);
struct CSynchronizedBlockCameraCtrl;
void fn_710002fb80(void*, f32);
void fn_7100763f9c(CSynchronizedBlockCameraCtrl*, f32);
void fn_71007642d4(CSynchronizedBlockCameraCtrl* self, f32 t, bool flag);

struct UnkCamVec3_ovb0020 {
    f32 x, y, z;
};

struct UnkMetroidCameraCtrl_ovb0020 {
    u8 unk_0x0[0x2b8];
    UnkCamVec3_ovb0020 field_0x2b8;
    UnkCamVec3_ovb0020 field_0x2c4;
    u8 unk_0x2d0[0x34];
    u32 field_0x304;
    f32 field_0x308;
};

void fn_7100935d40(UnkMetroidCameraCtrl_ovb0020* self) {
    self->field_0x304 = 3;
    f32 now = fn_7100080eac(g_unk_7101d47328);
    self->field_0x308 = now;
    self->field_0x2c4.x = 0.0f;
    self->field_0x2b8.x = 0.0f;
    self->field_0x2c4.y = 1e-5f;
    self->field_0x2c4.z = 0.0f;
    self->field_0x2b8.y = 1e-5f;
    self->field_0x2b8.z = 0.0f;
}

struct UnkCamData_ovb0020 {
    u8 unk_0x0[0x40];
    f32 distance;
};

struct UnkCamDataRef_ovb0020 {
    u8 unk_0x0[0x10];
    UnkCamData_ovb0020* camera;
};

struct UnkCamStartArg_ovb0020 {
    u8 unk_0x0[0x20];
    UnkXformNode node;
};

struct UnkMetroidCameraStart_ovb0020 {
    void** vtable;
    u8 unk_0x8[0x38];
    f32 field_0x40;
    u8 unk_0x44[0x2c];
    f32 field_0x70;
    u8 unk_0x74[0x44];
    UnkCamDataRef_ovb0020* ref;  // 0xb8
    u8 unk_0xc0[0x190];
    f32 field_0x250;
    f32 field_0x254;
    f32 field_0x258;
    f32 field_0x25c;
    u8 unk_0x260[0x58];
    UnkVec3f field_0x2b8;
    UnkVec3f field_0x2c4;
    u8 unk_0x2d0[0x30];
    u32 field_0x300;
    u8 unk_0x304[0xbc];
    f32 requested;  // 0x3c0
    f32 field_0x3c4;

    void vf_0x130() { reinterpret_cast<void (*)(UnkMetroidCameraStart_ovb0020*)>(vtable[0x130 / 8])(this); }
};

void fn_710093484c(UnkMetroidCameraStart_ovb0020* self, UnkCamStartArg_ovb0020* arg, bool flag) {
    f32 initial = self->requested > 0.0f ? self->requested : 2250.0f;
    self->field_0x258 = initial;
    self->field_0x254 = initial;
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), flag);
    f32 distance = self->requested;
    self->field_0x3c4 = 0.0f;
    if (!(distance > 0.0f)) {
        UnkCamData_ovb0020* camera;
        if (self->ref && (camera = self->ref->camera))
            distance = camera->distance;
        else
            distance = 1665.8074f;
    }
    self->field_0x258 = distance;
    self->field_0x70 = distance;
    self->field_0x254 = self->field_0x40;
    self->field_0x250 = self->field_0x40;
    self->field_0x300 = 2;
    UnkXformNode* node = &arg->node;
    if (fn_7100206154(node).z < 3000.0f) {
        f32 z = fn_7100206154(node).z;
        self->field_0x254 = z;
        self->field_0x250 = z;
    }
    self->vf_0x130();
    self->field_0x2b8 = self->field_0x2c4;
    if (fn_7100206154(node).z < 500.0f) {
        self->field_0x25c = 1.0f;
        self->field_0x2b8.x = 0.0f;
    }
}

struct UnkMetroidCameraUpdate_ovb0020 {
    u8 unk_0x0[0x38];
    UnkVec3f start_pos;  // 0x38
    UnkVec3f start_dir;  // 0x44
    u8 unk_0x50[0x18];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 unk_0x80[0x20d];
    bool field_0x28d;
    u8 unk_0x28e[0x72];
    u32 field_0x300;
    u32 field_0x304;
    f32 field_0x308;
    u8 unk_0x30c[4];
    u8 field_0x310[0xb4];
    f32 field_0x3c4;
};

void fn_7100935d94(UnkMetroidCameraUpdate_ovb0020* self, f32 dt) {
    fn_710002fb80(self->field_0x310, dt);
    f32 v = self->field_0x3c4;
    if (v > 0.0f) {
        v += dt * -100.0f;
        self->field_0x3c4 = v;
        if (v <= 0.0f)
            self->field_0x3c4 = 0.0f;
    } else if (v < 0.0f) {
        v = dt * 100.0f + v;
        self->field_0x3c4 = v;
        if (v >= 0.0f)
            self->field_0x3c4 = 0.0f;
    }
    CSynchronizedBlockCameraCtrl* base = reinterpret_cast<CSynchronizedBlockCameraCtrl*>(self);
    fn_7100763f9c(base, dt);
    if (u32 n = self->field_0x300) {
        if (self->field_0x28d) {
            self->start_pos = self->pos;
            self->start_dir = self->dir;
            n = 0;
        } else {
            n--;
        }
        self->field_0x300 = n;
    }
    if (self->field_0x304) {
        if (fn_7100080eac(g_unk_7101d47328) - self->field_0x308 > 1.0f) {
            self->field_0x304 = 0;
            return;
        }
        self->field_0x304--;
        fn_71007642d4(base, 0.0f, true);
        self->start_pos = self->pos;
        self->field_0x308 = fn_7100080eac(g_unk_7101d47328);
    }
}
