// Unit 1728 (timeline motion): an animation's frames turned into seconds through its table of time
// keys, found by binary search. Names and layouts are placeholders.
#include <algorithm>

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
struct UnkAnimClock_b8af0c {
    u8 pad_0x0[0x24];
    s32 frame_count;
    f32 length;
    u8 pad_0x2c[0x2c];
    UnkTimeKeys keys;  // 0x58
};

f32 fn_7100b8af0c(f32 frames, const UnkAnimTiming* timing) {
    if (frames == 0.0f)
        return 0.0f;
    auto* clock = reinterpret_cast<UnkAnimClock_b8af0c*>(const_cast<UnkAnimTiming*>(timing));
    if ((f32)clock->frame_count == frames)
        return clock->length;
    UnkTimeKeys* keys = &clock->keys;
    if (keys->count == 1)
        return frames / GetTimeKeyAtGrowingWithDefaults(keys, 0)->frame_time;
    // The first key past `frames`; the one before it holds it.
    UnkTimeKey* after = std::lower_bound(keys->data, keys->data + keys->count, frames,
                                         [](const UnkTimeKey& key, f32 value) { return key.time <= value; });
    const UnkTimeKey& key = after[-1];
    return key.frame + (frames - key.time) / key.frame_time;
}
