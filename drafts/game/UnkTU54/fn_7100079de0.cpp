// Unit 54: the product of two quaternions (x, y, z, w), returned in s0..s3. Names and layouts are placeholders.
#include "types.h"

struct UnkQuatMulVal {
    f32 x, y, z, w;
};

UnkQuatMulVal fn_7100079de0(const UnkQuatMulVal* a, const UnkQuatMulVal* b) {
    UnkQuatMulVal r;
    r.x = a->x * b->w + a->y * b->z - a->z * b->y + a->w * b->x;
    r.y = b->y * a->w + (b->w * a->y - b->z * a->x + a->z * b->x);
    r.z = b->z * a->w + (b->w * a->z + (a->x * b->y - a->y * b->x));
    r.w = b->w * a->w + (b->x * -a->x - a->y * b->y - b->z * a->z);
    return r;
}
