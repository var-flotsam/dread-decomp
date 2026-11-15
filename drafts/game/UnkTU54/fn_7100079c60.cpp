// Unit 54: a rotation matrix from a direction and an up vector (rows: side, up', direction). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkLookVec3 {
    f32 x, y, z;
};

UnkMatrix44 fn_7100079c60(const UnkLookVec3* dir, const UnkLookVec3* up) {
    UnkMatrix44 r;
    f32 sx = up->y * dir->z - up->z * dir->y;
    f32 sy = up->z * dir->x - dir->z * up->x;
    f32 sz = dir->y * up->x - up->y * dir->x;
    f32 inv = 1.0f / (sqrtf(sx * sx + sy * sy + sz * sz) + 0.0001f);
    sx = sx * inv;
    sy = sy * inv;
    sz = sz * inv;
    f32 ux = dir->y * sz - dir->z * sy;
    f32 uy = dir->z * sx - sz * dir->x;
    f32 uz = sy * dir->x - dir->y * sx;
    f32 inv2 = 1.0f / (sqrtf(ux * ux + uy * uy + uz * uz) + 0.0001f);
    r.m[0] = sx;
    r.m[1] = sy;
    r.m[2] = sz;
    r.m[3] = 0.0f;
    r.m[4] = ux * inv2;
    r.m[5] = uy * inv2;
    r.m[6] = uz * inv2;
    r.m[7] = 0.0f;
    r.m[8] = dir->x;
    r.m[9] = dir->y;
    r.m[10] = dir->z;
    r.m[11] = 0.0f;
    r.m[12] = 0.0f;
    r.m[13] = 0.0f;
    r.m[14] = 0.0f;
    r.m[15] = 1.0f;
    return r;
}
