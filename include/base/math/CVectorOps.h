#pragma once

// Inline 3D vector operations on base::math::CVector3D, in the forms the original inlines them.
// CVector3D stays a plain aggregate (returned in s0-s2, brace-initialised), so these are free
// functions. Each expression is written in the order the original evaluates it: the compiler
// neither contracts nor reassociates float arithmetic, so `a.x * b.x + a.y * b.y + a.z * b.z` is
// summed left to right, and a scale takes the vector component first (`v.x * s`).
//
// They are plain `inline`, not `static inline`: a static helper with reference parameters has
// them promoted to loaded values before it is inlined, which moves the loads; matched callers
// show the original's helpers were not. DistanceSq is written out (`(a.x - b.x) * (a.x - b.x)`)
// rather than as LengthSq(a - b): the two schedule differently, and the original uses the first.
//
// Not here: the 3D-to-2D projection (fn_710007b430, CVector.h) and the 4D operations, which the
// original calls out of line; and lerp, which the original writes in several operand orders.

#include <math.h>

#include "base/math/CVector.h"

namespace base::math {

inline CVector3D operator+(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.x + b.x, a.y + b.y, a.z + b.z};
}

inline CVector3D operator-(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.x - b.x, a.y - b.y, a.z - b.z};
}

inline CVector3D operator-(const CVector3D& v) {
    return CVector3D{-v.x, -v.y, -v.z};
}

inline CVector3D operator*(const CVector3D& v, f32 s) {
    return CVector3D{v.x * s, v.y * s, v.z * s};
}

inline f32 Dot(const CVector3D& a, const CVector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 LengthSq(const CVector3D& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline f32 Length(const CVector3D& v) {
    return sqrtf(LengthSq(v));
}

// The squared distance between two points.
inline f32 DistanceSq(const CVector3D& a, const CVector3D& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

// The distance between two points: the length of their difference.
inline f32 Distance(const CVector3D& a, const CVector3D& b) {
    return Length(a - b);
}

inline CVector3D Cross(const CVector3D& a, const CVector3D& b) {
    return CVector3D{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// The engine's safe normalisation: the reciprocal of the length plus a small epsilon, so a zero
// vector stays zero instead of dividing by zero.
inline f32 InvLengthEps(const CVector3D& v) {
    return 1.0f / (Length(v) + 0.0001f);
}

inline CVector3D Normalized(const CVector3D& v) {
    return v * InvLengthEps(v);
}

}  // namespace base::math
