// CCameraFX_RandShake's unit (tools/tu_map.py unit 2699): the random shake offset. Its strength is the
// curve sFunction read at the share of the run that has passed (field_0x50 over fTime; fn_7100076020 as
// in src/game/CCameraFX_Shake/CCameraFX_Shake_Falloff_ovb0108.cpp), 1 without a curve; the offset
// (field_0x68, field_0x74) is three random values in [-fDistance, fDistance] and one between fTwist and
// its negation, each times the strength. fn_71010f27c0 resets and rolls it after the base reset
// (fn_71010f3e94).
#include "base/global/CStrId.h"
#include "game/CCameraFX_Shake.h"
#include "game/UnkTU53.h"
#include "reflected/CCameraFX_RandShake.h"
#include "types.h"

extern "C" int rand();

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

void fn_71010f27c0(CCameraFX_RandShake* self) {
    fn_71010f3e94(self);
    self->field_0x68.x = 0.0f;
    self->field_0x68.y = 0.0f;
    self->field_0x68.z = 0.0f;
    self->field_0x74 = 0.0f;
    Roll(self, CurveStrength(self));
}
