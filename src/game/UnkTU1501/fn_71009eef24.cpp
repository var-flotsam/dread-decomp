// Unit 1501 (CSamusMovement area): fn_71009eef24 gives the signed angle, in units of 0.4636476 rad,
// between the owner's facing (the transform's third column, in the xy plane) and the direction from
// `from` to `to`, or 0 when either has about no x. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkTransformOwner_l9eef24 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkSelf_l9eef24 {
    u8 pad_0x0[0x30];
    UnkTransformOwner_l9eef24* owner;  // 0x30
};

// Whether v is outside [-0.001, 0.001] (or not a number).
static inline bool NotNearZero(f32 v) {
    return !(v + 0.001f >= 0.0f) || !(v - 0.001f <= 0.0f);
}

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    return v <= lo ? lo : (v >= hi ? hi : v);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
f32 fn_71009eef24(UnkSelf_l9eef24* self, const CVector3D* from, const CVector3D* to) {
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 facingX = m.m[2];
    f32 facingY = m.m[6];
    if (!NotNearZero(facingX))
        return 0.0f;
    if (!NotNearZero(to->x - from->x))
        return 0.0f;

    f32 fx = facingX;
    f32 fy = facingY;
    f32 fz = 0.0f;
    f32 len = sqrtf(fx * fx + fy * fy + fz * fz);
    fx = fx / len;
    fy = fy / len;
    fz = fz / len;

    f32 dx = to->x - from->x;
    f32 dy = to->y - from->y;
    f32 dz = 0.0f;
    f32 t = (fx * dx + fy * dy + fz * dz) / (dx * dx + dy * dy + dz * dz);
    f32 px = dx * t;
    f32 py = dy * t;
    f32 pz = dz * t;
    f32 plen = sqrtf(px * px + py * py + pz * pz);
    px = px / plen;
    py = py / plen;
    pz = pz / plen;

    f32 sign = fy * px - fx * py >= 0.0f ? -1.0f : 1.0f;
    f32 side = fx * sign;
    f32 cosine = fx * px + fy * py + fz * pz;
    return acosf(Clamp(cosine, -1.0f, 1.0f)) * side / 0.4636476f;
}
