// Translation unit 209 of data/tu_map.csv (0x710016e03c-0x710016e75c, no class hint): a spawn volume
// with a transform matrix at +0x1c, a size at +0x68, its shape at +0x8c (0 a point, 1 a disc in
// the x-z plane spread along y, 2 a sphere, 3 a cube; anything else a box) and the inner radius as
// a fraction at +0x90.
// Holds fn_710016e468 (a random point in the volume). Names and layouts are placeholders.
#include <math.h>
#include <stdlib.h>

#include <algorithm>

#include "game/UnkXformNode.h"
#include "types.h"

// The matrix type of fn_7100078170 (as src/game/CExplosionGfx.cpp names it).
struct UnkMat4f {
    f32 m[4][4];
};
// The point `v` transformed by the matrix.
UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);

struct UnkSpawnVolume {
    u8 unk_0x0[0x1c];
    UnkMat4f matrix;
    u8 unk_0x5c[0xc];
    f32 size[3];
    u8 unk_0x74[0x18];
    s32 shape;
    f32 inner;
};
static_assert(__builtin_offsetof(UnkSpawnVolume, shape) == 0x8c, "UnkSpawnVolume placeholder layout");

// A random value in [0, 1].
static inline f32 RandUnit() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

// Component-wise products.
static inline UnkVec3f operator*(const UnkVec3f& a, f32 s) {
    return UnkVec3f(a.x * s, a.y * s, a.z * s);
}
static inline UnkVec3f operator*(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x * b.x, a.y * b.y, a.z * b.z);
}

// A random point in the volume, transformed by its matrix (its scale applied twice: once through
// fn_7100076cec, once through the matrix).
// NON_MATCHING(minor): register allocation (the original keeps the matrix pointer in x21 next to
// the volume in x20, so it saves one more register and its frame slots shift), the radius products
// with their operands swapped (r * x for x * r), and the three moves of the cube's fallback point
// in another order.
UnkVec3f fn_710016e468(const UnkSpawnVolume* self) {
    UnkVec3f v;
    if (self->shape == 0) {
        v = UnkVec3f(0.0f, 0.0f, 0.0f);
    } else {
        f32 x = RandUnit() * 2.0f - 1.0f;
        f32 y = RandUnit() * 2.0f - 1.0f;
        f32 z = RandUnit() * 2.0f - 1.0f;
        UnkVec3f d(x, y, z);
        if (self->shape == 3) {
            f32 hi = std::max(std::max(d.x, d.y), d.z);
            f32 lo = std::min(std::min(d.x, d.y), d.z);
            f32 m = std::max(-lo, hi);
            if (m > 0.0f)
                d = d * (1.0f / m);
            else
                d = UnkVec3f(0.0f, 0.0f, 1.0f);
        } else if (self->shape == 2) {
            d = d * (1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f));
        } else if (self->shape == 1) {
            f32 len = sqrtf(d.x * d.x + d.z * d.z);
            if (len != 0.0f) {
                d.x = d.x / len;
                d.z = d.z / len;
            }
        }
        f32 inner = self->inner * self->inner;
        f32 t = RandUnit();
        f32 r = sqrtf(inner + (1.0f - inner) * t);
        v = d * r;
    }
    UnkVec3f p = UnkVec3f(v.x * self->size[0], v.y * self->size[1], v.z * self->size[2]) *
                 fn_7100076cec(reinterpret_cast<UnkXform*>(const_cast<UnkMat4f*>(&self->matrix)));
    return fn_7100078170(&self->matrix, &p);
}
