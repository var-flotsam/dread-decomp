// Unit 1141's static initialiser (helpers in game/UnkTUInit.h): the header, then nine motion-part
// rules, each two sets of motion parts (bits 1, 2 and 4; none or all of them, or one) and a mode.
// Names and layouts are placeholders.
#include <new>

#include "game/UnkTU1141.h"
#include "game/UnkTUInit.h"
#include "types.h"

struct UnkFlags_l714ec0 {
    u32 bits;
    UnkFlags_l714ec0() : bits(0) {}
    UnkFlags_l714ec0(u32 b) : bits(b) {}
    UnkFlags_l714ec0(const UnkFlags_l714ec0& o) : bits(o.bits) {}
};

inline const UnkFlags_l714ec0& UnkFlagsNone_l714ec0() {
    static UnkFlags_l714ec0 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_l714ec0& UnkFlagsAll_l714ec0() {
    static UnkFlags_l714ec0 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_l714ec0().bits & 7);
    return s_all;
}

// A rule over two sets of motion parts (0x14 bytes).
struct UnkMotionRule_l714ec0 {
    UnkFlags_l714ec0 from;
    UnkFlags_l714ec0 to;    // 0x4
    s32 mode;               // 0x8
    s32 field_0xc;
    bool field_0x10;
    bool field_0x11;

    UnkMotionRule_l714ec0(UnkFlags_l714ec0 from, UnkFlags_l714ec0 to, s32 mode)
        : from(from), to(to), mode(mode), field_0xc(0), field_0x10(false), field_0x11(false) {}
};
static_assert(sizeof(UnkMotionRule_l714ec0) == 0x14, "UnkMotionRule_l714ec0 layout");

// The header (declared as its first word in src/game/UnkUnit1141/UnkUnit1141_b42.cpp) and the rules
// (declared as bytes where other code reads them), all built in place here.
extern u32 g_unk_717270ec80 __attribute__((visibility("hidden")));
extern u8 g_unk_717270ebc8;
extern u8 g_unk_717270ebdc;
extern u8 g_unk_717270ec04;
extern u8 g_unk_717270ec18;
extern u8 g_unk_717270ec2c;
extern u8 g_unk_717270ec40;
extern u8 g_unk_717270ec54;

void fn_7100714ec0() {
    InitHeader(reinterpret_cast<UnkTUHeader&>(g_unk_717270ec80));
    new (&g_unk_717270ebc8) UnkMotionRule_l714ec0(UnkFlagsNone_l714ec0(), UnkFlagsAll_l714ec0(), 1);
    new (&g_unk_717270ebdc) UnkMotionRule_l714ec0(UnkFlagsNone_l714ec0(), UnkFlags_l714ec0(2), 1);
    new (&g_unk_717270ebf0) UnkMotionRule_l714ec0(UnkFlagsNone_l714ec0(), UnkFlags_l714ec0(4), 1);
    new (&g_unk_717270ec04) UnkMotionRule_l714ec0(UnkFlagsAll_l714ec0(), UnkFlagsNone_l714ec0(), 2);
    new (&g_unk_717270ec18) UnkMotionRule_l714ec0(UnkFlags_l714ec0(2), UnkFlagsNone_l714ec0(), 2);
    new (&g_unk_717270ec2c) UnkMotionRule_l714ec0(UnkFlags_l714ec0(4), UnkFlagsNone_l714ec0(), 2);
    new (&g_unk_717270ec40) UnkMotionRule_l714ec0(UnkFlagsAll_l714ec0(), UnkFlagsAll_l714ec0(), 1);
    new (&g_unk_717270ec54) UnkMotionRule_l714ec0(UnkFlags_l714ec0(2), UnkFlags_l714ec0(2), 1);
    new (&g_unk_717270ec68) UnkMotionRule_l714ec0(UnkFlags_l714ec0(4), UnkFlags_l714ec0(4), 1);
}
