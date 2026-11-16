// Unit 55 (0x710007b430-0x710007bb30, no class hint): the dot product of two 4D vectors
// (base/math/CVector4D.h; called through Dot in base/math/CVectorOps.h).
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

f32 fn_710007b590(const CVector4D* a, const CVector4D* b) {
    return b->x * a->x + b->y * a->y + b->z * a->z + b->w * a->w;
}
