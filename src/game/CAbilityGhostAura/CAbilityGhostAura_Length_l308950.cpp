// CAbilityGhostAura unit: the length of the translation of the motion that the timeline reports over its whole
// duration. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkFlags_l308950 {
    u32 bits;
    UnkFlags_l308950() : bits(0) {}
    UnkFlags_l308950(u32 b) : bits(b) {}
    UnkFlags_l308950(const UnkFlags_l308950& o) : bits(o.bits) {}
};

inline const UnkFlags_l308950& UnkFlagsNone_l308950() {
    static UnkFlags_l308950 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_l308950& UnkFlagsAll_l308950() {
    static UnkFlags_l308950 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_l308950().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

f32 fn_7100308950(UnkTimeline* timeline) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_l308950 flags = UnkFlagsAll_l308950();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    f32 x = motion.m[3];
    f32 y = motion.m[7];
    f32 z = motion.m[11];
    return sqrtf(x * x + y * y + z * z);
}
