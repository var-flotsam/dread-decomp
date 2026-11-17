#pragma once

// A position of three floats that functions return in memory: its copy constructor is user-provided, so
// it is not trivially copyable and comes back through x8 (fn_7100979a60, fn_710045bd10, fn_71008db1d8).
// The name is a placeholder.
#include "types.h"

struct UnkVec3Bits {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3Bits() {}
    UnkVec3Bits(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3Bits(const UnkVec3Bits& other) : x(other.x), y(other.y), z(other.z) {}

    // Copies three floats as their bits, never through a float register: all three words are read
    // before any is written (fn_710045bd10, fn_71008db1d8).
    static UnkVec3Bits CopyBits(const f32& x, const f32& y, const f32& z) {
        u32 words[3];
        __builtin_memcpy(&words[0], &x, sizeof(f32));
        __builtin_memcpy(&words[1], &y, sizeof(f32));
        __builtin_memcpy(&words[2], &z, sizeof(f32));
        UnkVec3Bits result;
        __builtin_memcpy(&result, words, sizeof(words));
        return result;
    }
};
static_assert(sizeof(UnkVec3Bits) == 0xc, "UnkVec3Bits layout");
