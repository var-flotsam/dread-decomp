#pragma once

#include "types.h"

// Returned in memory (x8): the copy constructor is user-provided (as in
// CKraidBouncingCreaturesMovementComponent_3.cpp).
// Returned in memory (x8): the copy constructor is user-provided.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};
