#pragma once

#include "types.h"

// The vector the argument's object returns from its virtual function at 0x68.
// (Returned in memory: its copy constructor is user-provided.)
struct UnkVector3 {
    f32 x, y, z;

    __attribute__((always_inline)) UnkVector3(const UnkVector3& other) : x(other.x), y(other.y), z(other.z) {}
};
