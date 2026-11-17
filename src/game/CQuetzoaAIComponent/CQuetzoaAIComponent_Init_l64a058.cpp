// CQuetzoaAIComponent's unit (tools/tu_map.py, unit 973): runs the base's step (fn_71003ac9ec), then reads
// the motion's end (+0x2c) of two of the owner's (+0x30) timelines into +0x14aec and +0x14af0.
// Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkMotionRule.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkStruct_7101176ea8;

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

struct UnkQuetzoaOwner_l64a058 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

// The ends of the two timelines' motions.
struct UnkQuetzoaMotionEnds_l64a058 {
    f32 first;
    f32 second;
};

struct UnkQuetzoaAI_l64a058 {
    u8 pad_0x0[0x30];
    UnkQuetzoaOwner_l64a058* owner;  // 0x30
    u8 pad_0x38[0x14aec - 0x38];
    UnkQuetzoaMotionEnds_l64a058 ends;  // 0x14aec
};

void fn_71003ac9ec(UnkQuetzoaAI_l64a058* self);

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

void fn_710064a058(UnkQuetzoaAI_l64a058* self) {
    fn_71003ac9ec(self);
    if (!self->owner->actions)
        return;
    UnkQuetzoaMotionEnds_l64a058& ends = self->ends;
    MotionEnd(self->owner->actions, 0x809cf8f8ffb0ef84ull, &ends.first);
    MotionEnd(self->owner->actions, 0xa8e3580f3384f703ull, &ends.second);
}
