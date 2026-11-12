#pragma once

// A set of bits passed by value. Its copy constructor makes a caller pass the address of a copy, which is
// what the original's callers do. UnkMaskOf<Tag> is a kind's empty set and its full set (nine bits), each a
// static built on first use. Names are placeholders.
#include "types.h"

struct UnkMask {
    u32 bits;
    UnkMask() : bits(0) {}
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};

template <class T>
struct UnkMaskOf {
    static const UnkMask& None() {
        static UnkMask s;
        return s;
    }
    static const UnkMask& All() {
        static UnkMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkSubAreaMaskTag;
