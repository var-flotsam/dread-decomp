// CChozoWarriorDeflectorShieldAttack's unit (tools/tu_map.py, unit 729): the shield's height from its
// animation. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkTU1728.h"
#include "types.h"


// fn_7100494578: the shield's height (+0x4c): cleared, then, when the owner's actions (+0x220) have the
// timeline 0x04dc4c0c7b23af17 (fn_7101176e0c), the absolute z translation of the motion over the whole
// timeline (fn_7100b8b3bc), as fn_7100425840
// (src/game/CChozoCommanderAirChargeAttack/CChozoCommanderAirChargeAttack_Charge_ovb0113.cpp) does.
struct UnkStruct_7101176ea8;

struct UnkFlags_ovb0130 {
    u32 bits;
    UnkFlags_ovb0130() : bits(0) {}
    UnkFlags_ovb0130(u32 b) : bits(b) {}
    UnkFlags_ovb0130(const UnkFlags_ovb0130& o) : bits(o.bits) {}
};

inline const UnkFlags_ovb0130& UnkFlagsNone_ovb0130() {
    static UnkFlags_ovb0130 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_ovb0130& UnkFlagsAll_ovb0130() {
    static UnkFlags_ovb0130 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_ovb0130().bits & 7);
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

void fn_71007142e8(UnkStruct_71007142e8* self);

struct UnkShieldOwner_ovb0130 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkShieldAttack_ovb0130 {
    u8 pad_0x0[8];
    UnkShieldOwner_ovb0130* owner;
    u8 pad_0x10[0x4c - 0x10];
    f32 height;  // 0x4c
};

void fn_7100494578(UnkShieldAttack_ovb0130* self) {
    self->height = 0.0f;
    UnkStruct_7101176ea8* actions = self->owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline =
        static_cast<UnkTimeline*>(fn_7101176e0c(actions, 0x04dc4c0c7b23af17ull, 0x137e114c0de4fb8dull));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_ovb0130 flags = UnkFlagsAll_ovb0130();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    self->height = __builtin_fabsf(motion.m[11]);
}
