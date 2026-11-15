// Unit 54: builds the rotation matrix about a unit axis by an angle. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkAxisVec3 {
    f32 x, y, z;
};

void fn_7100079584(const UnkAxisVec3* axis, UnkMatrix44* out, f32 angle) {
    f32 s = sinf(angle);
    f32 c = cosf(angle);
    f32 t = 1.0f - c;
    for (int i = 0; i < 16; i++) out->m[i] = 0.0f;
    f32 z = axis->z;
    f32 x = axis->x, y = axis->y;
    f32 sz = s * z;
    f32 tx = t * x;
    f32 ty = t * y;
    f32 tz = t * z;
    f32 xtx = x * tx, xty = x * ty, ytx = y * tx;
    f32 xtz = x * tz, yty = y * ty;
    f32 sy = s * y, sx = s * x;
    out->m[0] = c + xtx;
    out->m[1] = xty - sz;
    out->m[4] = ytx + sz;
    out->m[2] = sy + xtz;
    out->m[5] = c + yty;
    out->m[6] = y * tz - sx;
    out->m[8] = tx * z - sy;
    out->m[9] = sx + z * ty;
    out->m[10] = c + z * tz;
    out->m[15] = 1.0f;
}
