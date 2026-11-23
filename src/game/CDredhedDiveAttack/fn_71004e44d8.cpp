// CDredhedDiveAttack's unit, fn_71004e44d8: samples the owner's timeline 0x884c8e254ba39908
// (fn_7101176d34) over its whole length with the motion fn_71007142e8 sets up (fn_7100b8b3bc), and keeps
// the motion's word at +0x6c at +0x4c. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "types.h"

struct UnkStruct_7101176ea8;

// The motion fn_71007142e8 initialises.
struct UnkMotion_l4e44d8 {
    u8 pad_0x0[0x6c];
    u32 field_0x6c;
};

struct UnkMotionTracks;

struct UnkDredhedOwner_l4e44d8 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkDredhedDiveAttack_l4e44d8 {
    u8 pad_0x0[0x8];
    UnkDredhedOwner_l4e44d8* owner;  // 0x8
    u8 pad_0x10[0x4c - 0x10];
    u32 field_0x4c;
};

void fn_71004e44d8(UnkDredhedDiveAttack_l4e44d8* self) {
    UnkStruct_7101176ea8* actions = self->owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline =
        static_cast<UnkTimeline*>(fn_7101176d34(actions, reinterpret_cast<void*>(0x884c8e254ba39908ull)));
    if (!timeline)
        return;
    UnkMotion_l4e44d8 motion;
    fn_71007142e8(reinterpret_cast<UnkStruct_71007142e8*>(&motion));
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    self->field_0x4c = motion.field_0x6c;
}
