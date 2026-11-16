// Unit 1728 (timeline motion): advances a timeline by `delta` seconds from `time` when the flags allow its
// direction: the motion between the two frames is gathered (fn_7100b8b3bc) and applied to the target; a looping
// timeline that runs past its end goes on from the start with what is left. Names and layouts are placeholders.
#include <math.h>
#include <algorithm>
#include "game/UnkTU1141.h"
#include "game/UnkTU1728.h"
#include "types.h"

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkTimeKey;
struct UnkTimeKeys;
UnkTimeKey* fn_7100b8ad14(UnkTimeKeys* self, u32 index);
void fn_7100714808(void* target, UnkStruct_71007142e8* motion, bool flag);

// A time key: from `time` on, the frame advances `rate` frames a second from `frame`.
struct UnkTimeKey_b8b0dc {
    f32 time;
    f32 frame;
    f32 rate;
};

struct UnkTimeKeys_b8b0dc {
    UnkTimeKey_b8b0dc* data;
    u32 count;
};

struct UnkTimeline_b8b0dc {
    u8 pad_0x0[0x14];
    s32 loop;  // 1: looping
    u8 pad_0x18[0xc];
    s32 frame_count;
    f32 length;
    u8 pad_0x2c[0x8];
    u32 directions;  // 1: forward, 2: backward
    u8 pad_0x38[0x20];
    UnkTimeKeys_b8b0dc keys;
};

static inline bool IsNear(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

// The frame at `time`, rounded to hundredths.
static inline f32 FrameAt(UnkTimeline_b8b0dc* self, f32 time) {
    if (time == 0.0f)
        return 0.0f;
    if (self->length == time)
        return self->frame_count;
    f32 frame;
    if (self->keys.count == 1) {
        UnkTimeKey_b8b0dc* key =
            reinterpret_cast<UnkTimeKey_b8b0dc*>(fn_7100b8ad14(reinterpret_cast<UnkTimeKeys*>(&self->keys), 0));
        frame = time * key->rate;
    } else {
        UnkTimeKey_b8b0dc* next =
            std::lower_bound(self->keys.data, self->keys.data + self->keys.count, time,
                             [](const UnkTimeKey_b8b0dc& key, f32 t) { return key.time <= t; });
        const UnkTimeKey_b8b0dc& key = next[-1];
        frame = key.frame + (time - key.time) * key.rate;
    }
    return roundf(frame * 100.0f) / 100.0f;
}

void fn_7100b8b0dc(UnkTimeline_b8b0dc* self, void* target, UnkMotionTracks* tracks, const u32* flags, f32 time,
                   f32 delta) {
    if (delta != 0.0f && (((*flags & 2) && (self->directions & 1)) || ((*flags & 4) && (self->directions & 2)))) {
        for (;;) {
            f32 end = delta + time;
            bool looping = self->loop == 1;
            f32 length = self->length;
            if (!looping && IsNear(end, length))
                end = length;
            bool overflow = end > length;
            f32 clamped = overflow ? length : end;

            UnkStruct_71007142e8 motion;
            fn_71007142e8(&motion);
            f32 from = FrameAt(self, time);
            f32 to = FrameAt(self, clamped);
            fn_7100b8b3bc(reinterpret_cast<UnkTimeline*>(self), &motion, tracks, flags, from, to);
            fn_7100714808(target, &motion, true);
            if (!(looping && overflow))
                break;

            f32 rest = (end - time) - (clamped - time);
            delta = IsNear(rest, 0.0f) ? 0.0f : rest;
            time = 0.0f;
        }
    }
}
