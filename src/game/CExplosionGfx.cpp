// CExplosionGfx (tools/tu_map.py unit 2552, fn_7100dded9c to fn_7100de277c). The field names come
// from its reflected members (src/game_s7/fn_7100ddf434.cpp). This file holds fn_7100de08f0: the
// explosion's spawn at a target, the main burst and then uAmount - 1 extra ones.
#include <stdlib.h>

#include "types.h"

// The vector and matrix types of fn_7100078170 (src/game_s1/fn_7100078170.cpp).
struct UnkVec3f {
    f32 x, y, z;
    UnkVec3f() {}
    UnkVec3f(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3f(const UnkVec3f& o) : x(o.x), y(o.y), z(o.z) {}
};
struct UnkMat4f {
    f32 m[4][4];
};
UnkVec3f fn_7100078170(const UnkMat4f* m, const UnkVec3f* v);
UnkMat4f fn_7100078d8c(f32 angle);  // the rotation about z by angle

struct CExplosionGfx {
    u8 pad_0x0[0xb8];
    f32 fMainExplosionScale;  // +0xb8
    f32 vRandomScale[2];      // +0xbc
    f32 fRadius;              // +0xc4
    f32 fRadiusAmplitude;     // +0xc8
    f32 fRandomArcAmplitude;  // +0xcc
    u32 uAmount;              // +0xd0
    f32 fLength;              // +0xd4
    f32 vRandomDelay[2];      // +0xd8
};

// What an explosion is spawned at: a position at +0xc, and delays (count at +0x30) that
// fn_7100ddecd8 appends to.
struct UnkExplosionTarget {
    u8 pad_0x0[0xc];
    UnkVec3f pos;
};
void fn_7100ddecd8(UnkExplosionTarget* target, f32 delay);
void fn_7100de0cb0(CExplosionGfx* self, UnkExplosionTarget* target, const UnkVec3f* pos, const UnkVec3f* scale,
                   u32 flags);

static inline f32 Rand01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }
static inline f32 RandRange(f32 lo, f32 hi) {
    f32 r = Rand01();
    return lo + (hi - lo) * r;
}
static inline bool IsNearZero(f32 v) { return v + -0.001f <= 0.0f && v + 0.001f >= 0.0f; }

static const f32 kTwoPi = 6.2831855f;

// Spawns a burst at the target at fMainExplosionScale.
static inline void SpawnBurst(CExplosionGfx* self, UnkExplosionTarget* target) {
    UnkVec3f scale(self->fMainExplosionScale, self->fMainExplosionScale, self->fMainExplosionScale);
    fn_7100de0cb0(self, target, &target->pos, &scale, 0);
}

// Spawns the main burst at the target, then uAmount - 1 more: each at a random distance (fRadius,
// varied by fRadiusAmplitude) turned by a random arc (spread evenly round the circle when there
// are several), and either queued with a random delay (vRandomDelay) or, without one, spawned at
// once.
void fn_7100de08f0(CExplosionGfx* self, UnkExplosionTarget* target) {
    SpawnBurst(self, target);
    u32 count = self->uAmount - 1;
    for (u32 i = 0; i < count; ++i) {
        f32 radius = self->fRadius;
        f32 distance = radius + radius * RandRange(-self->fRadiusAmplitude, self->fRadiusAmplitude);
        f32 arc = self->fRandomArcAmplitude;
        f32 even = count > 1 ? (f32)i / (f32)count * kTwoPi : 0.0f;
        f32 turn = RandRange(arc * -kTwoPi, arc * kTwoPi);
        UnkVec3f offset(distance, 0.0f, 0.0f);
        if (!IsNearZero(distance)) {
            UnkMat4f rotation = fn_7100078d8c(even + turn);
            offset = fn_7100078170(&rotation, &offset);
        }
        f32 min_delay = self->vRandomDelay[0];
        f32 max_delay = self->vRandomDelay[1];
        if (!IsNearZero(min_delay + max_delay))
            fn_7100ddecd8(target, RandRange(min_delay, max_delay));
        else
            SpawnBurst(self, target);
    }
}
