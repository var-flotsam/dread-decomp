#pragma once

#include "types.h"

// A request's mode (passed by value, through memory).
struct UnkFlags_7101176ea8 {
    u32 value;
    UnkFlags_7101176ea8(u32 v) : value(v) {}
    UnkFlags_7101176ea8(const UnkFlags_7101176ea8& o) : value(o.value) {}
    UnkFlags_7101176ea8& operator=(const UnkFlags_7101176ea8& o) {
        value = o.value;
        return *this;
    }
};
