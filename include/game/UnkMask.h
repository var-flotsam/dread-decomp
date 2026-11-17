#pragma once

// A set of bits passed by value. Its copy constructor makes a caller pass the address of a copy, which is
// what the original's callers do. UnkMaskOf<Tag> is a kind's empty set and its full set (nine bits), each a
// static built on first use. The music manager takes it twice: a request's kind (one bit, such as 1 << the
// stack mode) and the mask it applies under (fn_7100fd7b44, fn_7100fd7c3c, fn_7100fd82dc; the loading screen
// and the music events pass both). Names are placeholders.
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
struct UnkMusicMaskTag;  // the music manager's mask (its full set is g_unk_71726c4080)
