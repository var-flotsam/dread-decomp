#pragma once

// base::math::CVector4D. The name comes from reflection (refl_GetType<base::math::CVector4D>,
// fn_71000248c4); the layout is inferred from reflected default values (four consecutive
// floats, e.g. colours).

#include "types.h"

namespace base::math {

struct CVector4D {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
static_assert(sizeof(CVector4D) == 0x10, "CVector4D layout");

// CVector4D where the original passes or returns one in memory (x8, or the address of a copy):
// there its copy constructor is user-provided (a member-wise copy) and its default constructor
// zeroes it (the reflected constructor does). The plain CVector4D above stays an aggregate for the
// reflection code and the calls that pass one in s0-s3.
struct UnkVector4D : CVector4D {
    UnkVector4D() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }
    UnkVector4D(const UnkVector4D& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
};
static_assert(sizeof(UnkVector4D) == 0x10, "UnkVector4D layout");

}  // namespace base::math

// The vector's assignment (out of line; the reflected assignment of CVector4D calls it).
void fn_710007b518(base::math::CVector4D* self, const base::math::CVector4D* other);
