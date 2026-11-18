// Unit 54: a rotation frame from a direction and an up vector: rows side, corrected up, direction.
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/UnkSpawnVolume.cpp names it.
struct UnkMat4f {
    f32 m[4][4];
};

UnkMat4f fn_7100079c60(const UnkVec3f* from, const UnkVec3f* to) {
    UnkMat4f r;
    f32 sx = to->y * from->z - to->z * from->y;
    f32 sy = to->z * from->x - from->z * to->x;
    f32 sz = from->y * to->x - to->y * from->x;
    f32 inv = 1.0f / (sqrtf(sx * sx + sy * sy + sz * sz) + 0.0001f);
    sx = sx * inv;
    sy = sy * inv;
    sz = sz * inv;
    f32 ux = from->y * sz - from->z * sy;
    f32 uy = from->z * sx - sz * from->x;
    f32 uz = sy * from->x - from->y * sx;
    f32 inv2 = 1.0f / (sqrtf(ux * ux + uy * uy + uz * uz) + 0.0001f);
    r.m[0][3] = 0.0f;
    r.m[1][0] = 0.0f;
    r.m[1][1] = 0.0f;
    r.m[1][2] = 0.0f;
    r.m[1][3] = 0.0f;
    r.m[2][0] = 0.0f;
    r.m[2][1] = 0.0f;
    r.m[2][2] = 0.0f;
    r.m[2][3] = 0.0f;
    r.m[3][0] = 0.0f;
    r.m[3][1] = 0.0f;
    r.m[3][2] = 0.0f;
    r.m[3][3] = 1.0f;
    r.m[0][0] = sx;
    r.m[0][1] = sy;
    r.m[0][2] = sz;
    r.m[1][0] = ux * inv2;
    r.m[1][1] = uy * inv2;
    r.m[1][2] = uz * inv2;
    UnkVec3f dir(*from);
    r.m[2][0] = dir.x;
    r.m[2][1] = dir.y;
    r.m[2][2] = dir.z;
    return r;
}
