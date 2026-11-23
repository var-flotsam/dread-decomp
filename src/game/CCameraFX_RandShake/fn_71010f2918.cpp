// CCameraFX_RandShake's unit (tools/tu_map.py unit 2699): the shake's step. While it runs (not stopped
// and within fTime) it rolls a new random offset scaled by the curve's strength and the shake's
// falloff (fn_71010f3f20) and adds it to the camera position and to the value at `roll`; then runs the
// base step. The helpers are CCameraFX_RandShake_Offset_ovb0131.cpp's.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CCameraFX_Shake.h"
#include "game/UnkTU53.h"
#include "reflected/CCameraFX_RandShake.h"
#include "types.h"

extern "C" int rand();

using base::math::CVector3D;

struct UnkCameraShake_ovb0108;
struct UnkShakePos_ovb0108;

struct UnkMotionShake_l0f1e24;

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = RandUnit();
    return lo + (hi - lo) * r;
}

// The curve's value at the share of the run that has passed, 1 without a curve.
static inline f32 CurveStrength(CCameraFX_RandShake* self) {
    if (self->sFunction.data == &g_unk_7101d47108->empty)
        return 1.0f;
    f32 t = self->field_0x50 / self->fTime;
    return fn_7100076020(t, self->sFunction.Text());
}

static inline void Roll(CCameraFX_RandShake* self, f32 strength) {
    f32 a = self->fDistance;
    self->field_0x68.x = strength * RandRange(-a, a);
    a = self->fDistance;
    self->field_0x68.y = strength * RandRange(-a, a);
    a = self->fDistance;
    self->field_0x68.z = strength * RandRange(-a, a);
    f32 b = self->fTwist;
    self->field_0x74 = strength * RandRange(b, -b);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71010f2918(CCameraFX_RandShake* self, void* camera, CVector3D* pos, CVector3D* target, void* arg4,
                   f32* roll, f32 dt) {
    if (!self->field_0x54 && (self->fTime <= 0.0f || self->field_0x50 <= self->fTime)) {
        f32 strength = CurveStrength(self);
        f32 scale = fn_71010f3f20(reinterpret_cast<UnkCameraShake_ovb0108*>(self),
                                  reinterpret_cast<const UnkShakePos_ovb0108*>(pos), strength);
        Roll(self, scale);
        pos->x = self->field_0x68.x + pos->x;
        pos->y = self->field_0x68.y + pos->y;
        pos->z = self->field_0x68.z + pos->z;
        *roll = self->field_0x74 + *roll;
    }
    fn_71010f3eb0(reinterpret_cast<UnkMotionShake_l0f1e24*>(self), camera, pos, target, arg4, roll, dt);
}
