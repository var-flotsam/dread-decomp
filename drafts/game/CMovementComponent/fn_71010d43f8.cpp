// CMovementComponent unit: the height speed that takes `from` (moved by the motion the timeline gives over its
// whole length, through `m`) to the height `to_y` over the motion's time (at most max_time); the result keeps its
// default (zero velocity, unit scale, zero time) when the motion has no time. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "game/UnkXformNode.h"
#include "types.h"

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

// The motion fn_71007142e8 initialises: the time it takes at +0x6c.
struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x6c];
    f32 time;
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

struct UnkStepResult_l0d43f8 {
    UnkVec3f velocity;
    UnkVec3f scale;
    f32 time;

    UnkStepResult_l0d43f8() {
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        velocity.z = 0.0f;
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
        time = 0.0f;
    }
};

UnkVec3f fn_7100714d78(const UnkXform* self, const UnkMatrix44* m);

UnkStepResult_l0d43f8 fn_71010d43f8(void* self, UnkTimeline* timeline, const UnkVec3f* from, const UnkMatrix44* m,
                                    f32 to_y, f32 max_time) {
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    {
        UnkFlags_71007142e8 flags = UnkFlagsAll_71007142e8();
        fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    }
    UnkStepResult_l0d43f8 r;
    f32 t = motion.time;
    if (t > 1.1920929e-7f) {
        if (t > max_time)
            t = max_time;
        r.time = t;
        UnkVec3f d = fn_7100714d78(reinterpret_cast<const UnkXform*>(&motion), m);
        r.velocity.y = (to_y - (from->y + d.y)) / t;
    }
    return r;
}
