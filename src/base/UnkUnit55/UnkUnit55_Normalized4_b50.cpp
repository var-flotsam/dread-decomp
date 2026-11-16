// Unit 55 of tools/tu_map.py: returns a 4D vector divided by its length, or the vector itself when
// the length is zero (base/math/CVector4D.h; called through Normalized in base/math/CVectorOps.h).
// The result is returned in memory.
#include <math.h>

#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

UnkVector4D fn_710007b690(const CVector4D* v) {
    f32 len = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
    if (len != 0.0f)
        return UnkVector4D(v->x / len, v->y / len, v->z / len, v->w / len);
    return UnkVector4D(*v);
}
