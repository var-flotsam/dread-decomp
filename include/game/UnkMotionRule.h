#pragma once

// A set of motion parts (bits 1, 2 and 4: fn_71007142e8's motions) and unit 1141's rules over two such sets,
// which its initialiser (fn_7100714ec0) builds in place. Names and layouts are placeholders.
#include "types.h"

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

// No part and every part: function-local statics the original shares between units.
inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

// A rule over two sets of motion parts (0x14 bytes).
struct UnkMotionRule {
    UnkFlags_71007142e8 from;
    UnkFlags_71007142e8 to;  // 0x4
    s32 mode;                // 0x8
    s32 field_0xc;
    bool field_0x10;
    bool field_0x11;

    UnkMotionRule(UnkFlags_71007142e8 from, UnkFlags_71007142e8 to, s32 mode)
        : from(from), to(to), mode(mode), field_0xc(0), field_0x10(false), field_0x11(false) {}
};
static_assert(sizeof(UnkMotionRule) == 0x14, "UnkMotionRule layout");
