// CCameraFX_SteadyCamera's unit (tools/tu_map.py unit 1173): sways a direction twice, by a random
// amount within the ranges at +0x34/+0x38 and +0x3c/+0x40, each scaled by the curve at +0x48 at a
// random point, and normalizes it after each sway. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "game/UnkTU53.h"
#include "types.h"

extern "C" int rand();

using base::math::CVector3D;

// How far a sway may move the direction, sideways and up.
struct UnkSwayRange_l769408 {
    f32 x;
    f32 y;
};

struct UnkSteadyCamera_l769408 {
    u8 pad_0x0[0x34];
    UnkSwayRange_l769408 sway;   // 0x34
    UnkSwayRange_l769408 drift;  // 0x3c
    u8 pad_0x44[0x4];
    const char* curve;  // 0x48
};

static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = RandUnit();
    return (hi - lo) * r + lo;
}

// The curve's value at a random point between `lo` and `hi`, 1 without a curve.
static inline f32 CurveStrength(UnkSteadyCamera_l769408* self, f32 lo, f32 hi) {
    f32 strength = 1.0f;
    if (self->curve && *self->curve)
        strength = fn_7100076020(RandRange(lo, hi), self->curve);
    return strength;
}

// Moves the direction by a random amount within the range either way (times the strength), then
// normalizes it.
static inline void Sway(CVector3D* dir, f32 strength, const UnkSwayRange_l769408* range) {
    f32 x = range->x;
    if (x > 0.0f && range->y > 0.0f) {
        f32 dx = strength * RandRange(-x, x);
        f32 y = range->y;
        f32 dy = strength * RandRange(-y, y);
        dir->x += dx;
        dir->y += dy;
        dir->z += 0.0f;
        f32 inv = 1.0f / (sqrtf(dir->x * dir->x + dir->y * dir->y + dir->z * dir->z) + 0.0001f);
        f32 nx = dir->x;
        f32 ny = dir->y;
        f32 nz = dir->z;
        dir->x = nx * inv;
        dir->y = ny * inv;
        dir->z = nz * inv;
    }
}

void fn_7100769408(UnkSteadyCamera_l769408* self, void* /*camera*/, void* /*unused*/, CVector3D* dir) {
    Sway(dir, CurveStrength(self, 0.01f, 1.0f), &self->sway);
    Sway(dir, CurveStrength(self, 0.0f, 1.0f), &self->drift);
}
