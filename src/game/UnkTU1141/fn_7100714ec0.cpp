// Unit 1141's static initialiser (helpers in game/UnkTUInit.h): the header, then nine motion-part
// rules (game/UnkMotionRule.h), each two sets of motion parts (bits 1, 2 and 4; none or all of them, or one)
// and a mode. Names and layouts are placeholders.
#include <new>

#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTUInit.h"
#include "types.h"

void fn_7100714ec0() {
    InitHeader(g_unk_717270ec80);
    new (&g_unk_717270ebc8) UnkMotionRule(UnkFlagsNone_71007142e8(), UnkFlagsAll_71007142e8(), 1);
    new (&g_unk_717270ebdc) UnkMotionRule(UnkFlagsNone_71007142e8(), UnkFlags_71007142e8(2), 1);
    new (&g_unk_717270ebf0) UnkMotionRule(UnkFlagsNone_71007142e8(), UnkFlags_71007142e8(4), 1);
    new (&g_unk_717270ec04) UnkMotionRule(UnkFlagsAll_71007142e8(), UnkFlagsNone_71007142e8(), 2);
    new (&g_unk_717270ec18) UnkMotionRule(UnkFlags_71007142e8(2), UnkFlagsNone_71007142e8(), 2);
    new (&g_unk_717270ec2c) UnkMotionRule(UnkFlags_71007142e8(4), UnkFlagsNone_71007142e8(), 2);
    new (&g_unk_717270ec40) UnkMotionRule(UnkFlagsAll_71007142e8(), UnkFlagsAll_71007142e8(), 1);
    new (&g_unk_717270ec54) UnkMotionRule(UnkFlags_71007142e8(2), UnkFlags_71007142e8(2), 1);
    new (&g_unk_717270ec68) UnkMotionRule(UnkFlags_71007142e8(4), UnkFlags_71007142e8(4), 1);
}
