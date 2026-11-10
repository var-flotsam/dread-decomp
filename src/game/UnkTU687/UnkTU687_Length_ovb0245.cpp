// CChozoRobotSoldierUppercutAttack's unit (tools/tu_map.py unit 687): the length of the uppercut's motion: zero, then,
// when the owner's (+0x8) actions (+0x220) have the uppercut timeline (fn_7101176e0c), the length of the translation
// of the motion fn_71007142e8 sets up as that timeline reports it over its whole duration (fn_7100b8b3bc), as
// src/game/CChozoCommanderAirChargeAttack/CChozoCommanderAirChargeAttack_Charge_ovb0113.cpp builds the end
// offset. Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkStruct_7101176ea8;

struct UnkFlags_ovb0245 {
    u32 bits;
    UnkFlags_ovb0245() : bits(0) {}
    UnkFlags_ovb0245(u32 b) : bits(b) {}
    UnkFlags_ovb0245(const UnkFlags_ovb0245& o) : bits(o.bits) {}
};

inline const UnkFlags_ovb0245& UnkFlagsNone_ovb0245() {
    static UnkFlags_ovb0245 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_ovb0245& UnkFlagsAll_ovb0245() {
    static UnkFlags_ovb0245 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_ovb0245().bits & 7);
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

void* fn_7101176e0c(UnkStruct_7101176ea8* self, u64 a, u64 b);
void fn_71007142e8(UnkStruct_71007142e8* self);
void fn_7100b8b3bc(UnkTimeline* self, void* target, UnkMotionTracks* tracks, const u32* flags, f32 from, f32 to);

struct UnkUppercutOwner_ovb0245 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkUppercutLength_ovb0245 {
    u8 pad_0x0[8];
    UnkUppercutOwner_ovb0245* owner;
    u8 pad_0x10[0x2c];
    f32 length;  // 0x3c
};

void fn_710046a228(UnkUppercutLength_ovb0245* self) {
    self->length = 0.0f;
    UnkStruct_7101176ea8* actions = self->owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176e0c(actions, 0x98c6fdc80e86d316ull, ~0ull));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_ovb0245 flags = UnkFlagsAll_ovb0245();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    f32 x = motion.m[3];
    f32 y = motion.m[7];
    f32 z = motion.m[11];
    self->length = sqrtf(x * x + y * y + z * z);
}
