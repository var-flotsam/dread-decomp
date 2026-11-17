// behaviortree::CHyperDashAttackAction's unit (tools/tu_map.py, unit 662): keeps the owner and reads the
// motion's end (+0x2c) of two of its timelines into +0xbc and +0xc0. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkStruct_7101176ea8;

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none;
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

// The motion fn_71007142e8 initialises: the time it ends on at +0x2c.
struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

struct UnkHyperDashOwner_l44b060 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkHyperDashAttackAction_l44b060 {
    u8 pad_0x0[0x18];
    UnkHyperDashOwner_l44b060* owner;  // 0x18
    u8 pad_0x20[0xbc - 0x20];
    f32 field_0xbc;
    f32 field_0xc0;
};

// The end of the motion over the whole of the timeline `key`, when the owner has it.
static inline bool MotionEnd(UnkStruct_7101176ea8* actions, u64 key, f32* out) {
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(actions, reinterpret_cast<void*>(key)));
    if (!timeline)
        return false;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    *out = motion.field_0x2c;
    return true;
}

void fn_710044b060(UnkHyperDashAttackAction_l44b060* self, UnkHyperDashOwner_l44b060* owner) {
    self->owner = owner;
    self->field_0xbc = 0.0f;
    self->field_0xc0 = 0.0f;
    if (!owner->actions)
        return;
    MotionEnd(owner->actions, 0xb6437005e0c512cfull, &self->field_0xbc);
    MotionEnd(owner->actions, 0xd895e97b02768a14ull, &self->field_0xc0);
}
