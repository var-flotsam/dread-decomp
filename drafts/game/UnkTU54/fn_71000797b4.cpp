// Unit 54: the view matrix of an eye looking at a target with an up direction (the rotation onto
// the right, up and backward axes times the translation by minus the eye). Names and layouts are
// placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkVec3_197 {
    f32 x, y, z;
};

static inline f32 InvLength(f32 x, f32 y, f32 z) {
    return 1.0f / (sqrtf(x * x + y * y + z * z) + 0.0001f);
}

UnkMatrix44 fn_71000797b4(const UnkVec3_197* eye, const UnkVec3_197* target, const UnkVec3_197* up) {
    UnkMatrix44 rot;
    for (int i = 0; i < 16; i++) rot.m[i] = 0.0f;
    rot.m[15] = 1.0f;

    f32 dx = target->x - eye->x;
    f32 dy = target->y - eye->y;
    f32 dz = target->z - eye->z;
    f32 inv = InvLength(dx, dy, dz);
    f32 fx = dx * inv;
    f32 fy = dy * inv;
    f32 fz = dz * inv;

    f32 cx = fy * up->z - fz * up->y;
    f32 cy = fz * up->x - fx * up->z;
    f32 cz = fx * up->y - fy * up->x;
    inv = InvLength(cx, cy, cz);
    f32 sx = cx * inv;
    f32 sy = cy * inv;
    f32 sz = cz * inv;

    f32 ux = sy * fz - sz * fy;
    f32 uy = sz * fx - sx * fz;
    f32 uz = sx * fy - sy * fx;
    inv = InvLength(ux, uy, uz);

    rot.m[0] = sx;
    rot.m[1] = sy;
    rot.m[2] = sz;
    rot.m[4] = inv * ux;
    rot.m[5] = inv * uy;
    rot.m[6] = inv * uz;
    rot.m[8] = -fx;
    rot.m[9] = -fy;
    rot.m[10] = -fz;

    UnkMatrix44 move;
    for (int i = 0; i < 16; i++) move.m[i] = 0.0f;
    move.m[15] = 1.0f;
    move.m[10] = 1.0f;
    move.m[5] = 1.0f;
    move.m[0] = 1.0f;
    move.m[3] = -eye->x;
    move.m[7] = -eye->y;
    move.m[11] = -eye->z;
    return fn_71000127c8(rot, move);
}
