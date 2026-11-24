// CGoliathXBurstProjectionBombsAttack's unit (tools/tu_map.py, unit 835): when the owner (+0x8) has
// actions (+0x220), reads the motion's end (+0x2c) of the timelines named by g_unk_71726e55b8 and
// g_unk_71726e55c0 into +0x30 and +0x34. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/CGoliathXBurstProjectionBombsAttack.h"
#include "game/UnkMotionRule.h"
#include "game/UnkStruct_71007142e8.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkTimeline.h"
#include "types.h"

struct UnkStruct_7101176ea8;

// The motion fn_71007142e8 initialises: the time it ends on at +0x2c.
struct UnkMotionTracks;

struct UnkCls_710117af10;

// What this unit reads of a UnkCls_710117af10: its key (+0x30).
struct UnkGoliathXBombsName_l5858c4 {
    u8 pad_0x0[0x30];
    void* key;  // 0x30
};

struct UnkGoliathXBombsOwner_l5858c4 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

// The ends of the two timelines' motions.
struct UnkMotionEnds_l5858c4 {
    f32 first;
    f32 second;
};

struct UnkGoliathXBombsAttack_l5858c4 {
    u8 pad_0x0[0x8];
    UnkGoliathXBombsOwner_l5858c4* owner;  // 0x8
    u8 pad_0x10[0x30 - 0x10];
    UnkMotionEnds_l5858c4 ends;  // 0x30
};

// The end of the motion over the whole of the timeline `name`, when the owner has it.
static inline void MotionEnd(UnkStruct_7101176ea8* actions, UnkCls_710117af10* name, f32* out) {
    void* key = reinterpret_cast<UnkGoliathXBombsName_l5858c4*>(name)->key;
    UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(actions, key));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    *out = motion.xform.m[2][3];
}

void CGoliathXBurstProjectionBombsAttack_CacheTimelineMotionValues(UnkGoliathXBombsAttack_l5858c4* self) {
    if (!self->owner->actions)
        return;
    self->ends = UnkMotionEnds_l5858c4();
    MotionEnd(self->owner->actions, g_unk_71726e55b8, &self->ends.first);
    MotionEnd(self->owner->actions, g_unk_71726e55c0, &self->ends.second);
}
