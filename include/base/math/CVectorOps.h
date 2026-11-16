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
// The 4D operations are out of line in the original (unit 55, declared in CVector4D.h): the ones
// here only call them, so callers make the original's calls. Their results are UnkVector4D, returned
// in memory as the original returns them.
//
// Not here: the 3D-to-2D projection (fn_710007b430, CVector.h), which callers call out of line;
// and lerp, which the original writes in several operand orders.

#include <math.h>

#include "base/math/CVector.h"
#include "base/math/CVector4D.h"

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

// 2D: the same forms on base::math::CVector2D.

inline CVector2D operator+(const CVector2D& a, const CVector2D& b) {
    return CVector2D{a.x + b.x, a.y + b.y};
}

inline CVector2D operator-(const CVector2D& a, const CVector2D& b) {
    return CVector2D{a.x - b.x, a.y - b.y};
}

inline CVector2D operator*(const CVector2D& v, f32 s) {
    return CVector2D{v.x * s, v.y * s};
}

inline f32 Dot(const CVector2D& a, const CVector2D& b) {
    return a.x * b.x + a.y * b.y;
}

inline f32 LengthSq(const CVector2D& v) {
    return v.x * v.x + v.y * v.y;
}

inline f32 Length(const CVector2D& v) {
    return sqrtf(LengthSq(v));
}

// Normalises in place by dividing by the length, leaving a zero vector as it is. (The 2D
// normalisation divides; the 3D one above multiplies by an epsilon-guarded reciprocal.)
inline void Normalize(CVector2D& v) {
    f32 length = Length(v);
    if (length != 0.0f) {
        v.x = v.x / length;
        v.y = v.y / length;
    }
}

// 4D: calls to the original's out-of-line functions.

inline UnkVector4D operator+(const CVector4D& a, const CVector4D& b) {
    return fn_710007b5ec(&a, &b);
}

inline UnkVector4D operator-(const CVector4D& a, const CVector4D& b) {
    return fn_710007b618(&a, &b);
}

inline UnkVector4D operator*(const CVector4D& v, f32 s) {
    return fn_710007b5c0(&v, s);
}

inline CVector4D& operator*=(CVector4D& v, f32 s) {
    fn_710007b53c(&v, s);
    return v;
}

inline CVector4D& operator/=(CVector4D& v, f32 s) {
    fn_710007b568(&v, s);
    return v;
}

inline bool operator==(const CVector4D& a, const CVector4D& b) {
    return fn_710007b644(&a, &b);
}

inline f32 Dot(const CVector4D& a, const CVector4D& b) {
    return fn_710007b590(&a, &b);
}

// Divides by the length (no epsilon); a zero vector is returned as it is.
inline UnkVector4D Normalized(const CVector4D& v) {
    return fn_710007b690(&v);
}

// CVector4D's assignment, which the original calls out of line (an aggregate cannot have a
// user-provided operator= without losing its register passing).
inline void Assign(CVector4D& dst, const CVector4D& src) {
    fn_710007b518(&dst, &src);
}

}  // namespace base::math
