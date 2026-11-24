// Unit 1728 (timeline motion): seconds on an animation's clock turned into frames through its table of
// time keys, found by binary search, rounded to hundredths. Names and layouts are placeholders.
#include <algorithm>
#include <cmath>

#include "game/UnkTU1728.h"
#include "types.h"

struct UnkTimeKey {
    f32 frame;
    f32 time;
    f32 frame_time;
};
struct UnkTimeKeys {
    UnkTimeKey* data;
    u32 count;
};
UnkTimeKey* GetTimeKeyAtGrowingWithDefaults(UnkTimeKeys* self, u32 index);

// The part of an animation's timing these conversions read.
struct UnkAnimClock_b8aff0 {
    u8 pad_0x0[0x24];
    s32 frame_count;
    f32 length;
    u8 pad_0x2c[0x2c];
    UnkTimeKeys keys;  // 0x58
};

f32 fn_7100b8aff0(f32 seconds, const UnkAnimTiming* timing) {
    if (seconds == 0.0f)
        return 0.0f;
    auto* clock = reinterpret_cast<UnkAnimClock_b8aff0*>(const_cast<UnkAnimTiming*>(timing));
    if (clock->length == seconds)
        return (f32)clock->frame_count;
    UnkTimeKeys* keys = &clock->keys;
    f32 frames;
    if (keys->count == 1) {
        frames = GetTimeKeyAtGrowingWithDefaults(keys, 0)->frame_time * seconds;
    } else {
        // The first key past `seconds`; the one before it holds it.
        UnkTimeKey* after = std::lower_bound(keys->data, keys->data + keys->count, seconds,
                                             [](const UnkTimeKey& key, f32 value) { return key.frame <= value; });
        const UnkTimeKey& key = after[-1];
        frames = key.time + (seconds - key.frame) * key.frame_time;
    }
    return roundf(frames * 100.0f) / 100.0f;
}
