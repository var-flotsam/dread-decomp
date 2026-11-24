#pragma once

#include "game/UnkTimelineEventList.h"
#include "types.h"

struct UnkMotionTracks;

// A time key: the frame at `time`, and the time one frame lasts from there.
struct UnkTimeKey {
    f32 frame;
    f32 time;
    f32 frame_time;
};
struct UnkTimeKeys {
    UnkTimeKey* data;
    u32 count;
};
// The key at `index` (out of line).
UnkTimeKey* fn_7100b8ad14(UnkTimeKeys* self, u32 index);

// A motion's timeline: its frame count and last frame, which motions it has, its time keys, its tracks
// and its events. Names and layout are placeholders.
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    f32 last_frame;  // the frame at frame_count
    u8 pad_0x2c[0x8];
    u32 flags;  // bit 0: has a first motion, bit 1: has a second
    u8 pad_0x38[0x20];
    UnkTimeKeys keys;
    u8 pad_0x68[0x18];
    UnkMotionTracks* tracks;  // 0x80
    u8 pad_0x88[0xe8];
    UnkTimelineEventList events;
};
static_assert(__builtin_offsetof(UnkTimeline, keys) == 0x58, "UnkTimeline placeholder layout");
static_assert(__builtin_offsetof(UnkTimeline, tracks) == 0x80, "UnkTimeline placeholder layout");
static_assert(__builtin_offsetof(UnkTimeline, events) == 0x170, "UnkTimeline placeholder layout");
