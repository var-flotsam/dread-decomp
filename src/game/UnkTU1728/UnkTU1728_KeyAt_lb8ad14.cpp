// Unit 1728 (timeline motion): the time key at an index, growing the keys up to it with default keys
// (a frame lasting 30) when it is past the end: CRntVector::GrowAt written out. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkTimeKey;
struct UnkTimeKeys;

// A time key: the frame at `time`, and the time one frame lasts from there.
struct UnkTimeKey_b8ad14 {
    f32 frame = 0.0f;
    f32 time = 0.0f;
    f32 frame_time = 30.0f;

    ~UnkTimeKey_b8ad14() {}
};

UnkTimeKey* GetTimeKeyAtGrowingWithDefaults(UnkTimeKeys* self, u32 index) {
    auto* keys = reinterpret_cast<base::global::CRntVector<UnkTimeKey_b8ad14>*>(self);
    if (keys->field_0x14 && keys->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 keys.
        u32 cap = keys->capacity;
        if (cap <= index && keys->step != 0) {
            u32 grown = keys->GrownCapacity(index);
            if (cap < grown) {
                keys->Reserve(grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            keys->Reserve(n);
        for (u32 i = keys->count; i < n; ++i)
            new (&keys->data[i]) UnkTimeKey_b8ad14();
        keys->count = n;
    }
    return reinterpret_cast<UnkTimeKey*>(&keys->data[index]);
}
