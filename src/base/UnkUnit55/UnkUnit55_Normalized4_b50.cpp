// Unit 55 of tools/tu_map.py: returns a 4-component float vector divided by its length, or the vector
// itself when the length is zero. The vector has a user-provided copy, so it is returned in memory.
// Names are placeholders.
#include <math.h>
#include "types.h"

struct UnkVec4fN {
    f32 x, y, z, w;

    UnkVec4fN(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkVec4fN(const UnkVec4fN& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

UnkVec4fN fn_710007b690(const UnkVec4fN* v) {
    f32 len = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
    if (len != 0.0f)
        return UnkVec4fN(v->x / len, v->y / len, v->z / len, v->w / len);
    return *v;
}
