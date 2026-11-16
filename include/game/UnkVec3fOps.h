#pragma once

// The 3D vector operations of base/math/CVectorOps.h for UnkVec3f (game/UnkXformNode.h), in the
// same forms: plain inline (not static), reference parameters, each expression in the order the
// original evaluates it. UnkVec3f copies itself word by word and is returned in memory, so these
// return through x8 where the CVector3D ones return in s0-s2; the arithmetic is the same.
//
// Six sources in src/game_s1/ define a different struct of the same name (a member-wise float
// copy); they do not include this header.

#include <math.h>

#include "game/UnkXformNode.h"
#include "types.h"

inline UnkVec3f operator+(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline UnkVec3f operator-(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline UnkVec3f operator-(const UnkVec3f& v) {
    return UnkVec3f(-v.x, -v.y, -v.z);
}

inline UnkVec3f operator*(const UnkVec3f& v, f32 s) {
    return UnkVec3f(v.x * s, v.y * s, v.z * s);
}

inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline f32 LengthSq(const UnkVec3f& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline f32 Length(const UnkVec3f& v) {
    return sqrtf(LengthSq(v));
}

// The squared distance between two points.
inline f32 DistanceSq(const UnkVec3f& a, const UnkVec3f& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

// The distance between two points: the length of their difference.
inline f32 Distance(const UnkVec3f& a, const UnkVec3f& b) {
    return Length(a - b);
}

inline UnkVec3f Cross(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

// The engine's safe normalisation (base/math/CVectorOps.h): the reciprocal of the length plus a
// small epsilon.
inline f32 InvLengthEps(const UnkVec3f& v) {
    return 1.0f / (Length(v) + 0.0001f);
}

inline UnkVec3f Normalized(const UnkVec3f& v) {
    return v * InvLengthEps(v);
}
