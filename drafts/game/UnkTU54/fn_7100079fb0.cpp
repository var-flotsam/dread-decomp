#include <math.h>
#include "types.h"

struct UnkQuat_b0172 {
    f32 x, y, z, w;
};

UnkQuat_b0172 fn_7100079fb0(const UnkQuat_b0172* q) {
    f32 len = sqrtf(q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
    if (!(len <= 0.0f)) {
        UnkQuat_b0172 r;
        r.x = q->x / len;
        r.y = q->y / len;
        r.z = q->z / len;
        r.w = q->w / len;
        return r;
    }
    UnkQuat_b0172 id = {0.0f, 0.0f, 0.0f, 1.0f};
    return id;
}
