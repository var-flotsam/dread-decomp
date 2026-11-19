// CCameraFX_RandShake's unit (tools/tu_map.py unit 2699): the shake's step. While it runs (not stopped
// and within its duration) it rolls a new random offset scaled by the curve's strength and the shake's
// falloff (fn_71010f3f20) and adds it to the camera position and to the value at `roll`; then runs the
// base step. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkTU53.h"
#include "types.h"

extern "C" int rand();

using base::math::CVector3D;

struct UnkRandShakeCurve_ovb0131 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    union {
        u32 use_count;
    };
};

struct UnkRandShake_ovb0131 {
    u8 pad_0x0[0x34];
    f32 duration;  // 0x34
    u8 pad_0x38[0x18];
    f32 time;  // 0x50
    bool stopped;  // 0x54
    u8 pad_0x55[0x3];
    UnkRandShakeCurve_ovb0131* curve;  // 0x58
    f32 amplitude;  // 0x60
    f32 amplitude_w;  // 0x64
    CVector3D offset;  // 0x68
    f32 offset_w;  // 0x74
};

struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;
f32 fn_71010f3f20(UnkCameraShake_ovb0108* self, const UnkShakePos_ovb0108* pos, f32 scale);

struct UnkMotionShake_l0f1e24;
void fn_71010f3eb0(UnkMotionShake_l0f1e24* self, void* camera, CVector3D* pos, CVector3D* target, void* arg4,
                   void* arg5, f32 dt);

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = RandUnit();
    return lo + (hi - lo) * r;
}

// The curve's value at the elapsed share of the run, 1 without a curve.
static inline f32 CurveStrength(UnkRandShake_ovb0131* self) {
    UnkRandShakeCurve_ovb0131* curve = self->curve;
    if (curve == reinterpret_cast<UnkRandShakeCurve_ovb0131*>(&g_unk_7101d47108->empty))
        return 1.0f;
    f32 t = self->time / self->duration;
    ++curve->use_count;
    const char* text = self->curve->text;
    return fn_7100076020(t, text ? text : g_unk_710162e2ed);
}

static inline void Roll(UnkRandShake_ovb0131* self, f32 strength) {
    f32 a = self->amplitude;
    self->offset.x = strength * RandRange(-a, a);
    a = self->amplitude;
    self->offset.y = strength * RandRange(-a, a);
    a = self->amplitude;
    self->offset.z = strength * RandRange(-a, a);
    f32 b = self->amplitude_w;
    self->offset_w = strength * RandRange(b, -b);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010f2918(UnkRandShake_ovb0131* self, void* camera, CVector3D* pos, CVector3D* target, void* arg4,
                   f32* roll, f32 dt) {
    if (!self->stopped && (self->duration <= 0.0f || self->time <= self->duration)) {
        f32 strength = CurveStrength(self);
        f32 scale = fn_71010f3f20(reinterpret_cast<UnkCameraShake_ovb0108*>(self),
                                  reinterpret_cast<const UnkShakePos_ovb0108*>(pos), strength);
        Roll(self, scale);
        pos->x = self->offset.x + pos->x;
        pos->y = self->offset.y + pos->y;
        pos->z = self->offset.z + pos->z;
        *roll = self->offset_w + *roll;
    }
    fn_71010f3eb0(reinterpret_cast<UnkMotionShake_l0f1e24*>(self), camera, pos, target, arg4, roll, dt);
}
