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
    UnkVector4D(f32 x_, f32 y_, f32 z_, f32 w_) {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }
    UnkVector4D(const UnkVector4D& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
    explicit UnkVector4D(const CVector4D& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }
};
static_assert(sizeof(UnkVector4D) == 0x10, "UnkVector4D layout");

}  // namespace base::math

// The vector's operations, all out of line in the original (unit 55, 0x710007b518-0x710007b744);
// base/math/CVectorOps.h gives them operator forms. Those returning a vector return it in memory.
// The assignment (the reflected assignment of CVector4D calls it).
void fn_710007b518(base::math::CVector4D* self, const base::math::CVector4D* other);
// Scales in place, and divides in place.
void fn_710007b53c(base::math::CVector4D* self, f32 s);
void fn_710007b568(base::math::CVector4D* self, f32 s);
// The dot product.
f32 fn_710007b590(const base::math::CVector4D* a, const base::math::CVector4D* b);
// a * s, a + b, a - b.
base::math::UnkVector4D fn_710007b5c0(const base::math::CVector4D* a, f32 s);
base::math::UnkVector4D fn_710007b5ec(const base::math::CVector4D* a, const base::math::CVector4D* b);
base::math::UnkVector4D fn_710007b618(const base::math::CVector4D* a, const base::math::CVector4D* b);
// Whether all four components are equal.
bool fn_710007b644(const base::math::CVector4D* a, const base::math::CVector4D* b);
// The vector divided by its length, or the vector itself when the length is zero
// (src/base/UnkUnit55/UnkUnit55_Normalized4_b50.cpp).
base::math::UnkVector4D fn_710007b690(const base::math::CVector4D* v);
