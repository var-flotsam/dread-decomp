// CAbilityGhostAura unit: how far into an animation (by its key) the motion stays at or above a sideways
// threshold, capped for two animations. Names and layouts are placeholders.
#include <math.h>

#include "game/CAnimationComponent.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

struct UnkGhostAuraOwner_l312ed4 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* animation;  // 0x220
};

struct UnkGhostAuraEntity_l312ed4 {
    u8 pad_0x0[0x30];
    UnkGhostAuraOwner_l312ed4* owner;  // 0x30
};

struct UnkGhostAura_l312ed4 {
    u8 pad_0x0[0xc0];
    UnkGhostAuraEntity_l312ed4* entity;  // 0xc0
    u8 pad_0xc8[0x3d0];
    bool field_0x498;
};

// The two animations whose reach is capped get the cap; every other one its reach.
static inline f32 ForKey(u64 key, f32 reach, f32 cap) {
    return key == 0xa6530f738afab46c ? cap : key == 0x9072e6e052108693 ? cap : reach;
}

// Steps through the animation half a frame at a time until the motion's sideways offset falls under the
// threshold; the two capped animations never report more than 5 (2.5 while field_0x498 is set).
f32 fn_7100312ed4(UnkGhostAura_l312ed4* self, const u64* key, f32 threshold) {
    f32 reach = 0.0f;
    f32 cap;
    if (self->field_0x498) {
        cap = 2.5f;
    } else if (threshold <= 0.0f) {
        cap = 5.0f;
    } else {
        UnkGhostAuraEntity_l312ed4* entity = self->entity;
        if (!entity)
            return 0.0f;
        UnkStruct_7101176ea8* animation = entity->owner->animation;
        if (!animation)
            return 0.0f;
        UnkTimeline* timeline = static_cast<UnkTimeline*>(fn_7101176d34(animation, reinterpret_cast<void*>(*key)));
        if (!timeline)
            return 0.0f;
        s32 frames = timeline->frame_count;
        UnkMotionTracks* tracks = timeline->tracks;
        f32 end = frames;
        UnkStruct_71007142e8 motion;
        fn_71007142e8(&motion);
        f32 t = 0.0f;
        if (frames > 0) {
            while (t < end) {
                u32 flags = 2;
                fn_7100b8b3bc(timeline, &motion, tracks, &flags, t, end);
                if (fabsf(motion.m[11]) < threshold)
                    break;
                t += 0.5f;
            }
        }
        reach = t < end ? t : 0.0f;
        cap = reach < 5.0f ? reach : 5.0f;
    }
    return ForKey(*key, reach, cap);
}
