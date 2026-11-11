// CCameraFX_RandShake's unit (tools/tu_map.py unit 2699): the random shake offset. Its strength comes
// from the curve at +0x58 at the elapsed share of the run (+0x50 over +0x34; fn_7100076020 as in
// src/game/CCameraFX_Shake/CCameraFX_Shake_Falloff_ovb0108.cpp), 1 without a curve; the offset
// (+0x68..+0x74) is three random values in [-amplitude, amplitude] (+0x60) and one between the
// amplitude at +0x64 and its negation, each times the strength. fn_71010f27c0 resets and rolls it
// after the base reset (fn_71010f3e94). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCameraFX_Shake.h"
#include "game/UnkTU53.h"
#include "types.h"

extern "C" int rand();

struct UnkRandShakeCurve_ovb0131 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    union {
        u32 use_count;
    };
};

struct UnkRandShakeOffset_ovb0131 {
    f32 x, y, z;
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
    UnkRandShakeOffset_ovb0131 offset;  // 0x68
    f32 offset_w;  // 0x74
};

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

void fn_71010f27c0(UnkRandShake_ovb0131* self) {
    fn_71010f3e94(self);
    self->offset.x = 0.0f;
    self->offset.y = 0.0f;
    self->offset.z = 0.0f;
    self->offset_w = 0.0f;
    Roll(self, CurveStrength(self));
}
