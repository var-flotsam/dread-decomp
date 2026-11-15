// Unit 2647 (sound::CAudioPreset): End of the reflected iteration over the bank of sixteen presets.
// Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkResPathVec.h"
#include "types.h"

struct UnkAudioPresetPair_b0192;
void fn_71010396a8(UnkAudioPresetPair_b0192* self);

struct UnkAudioPresetPair_b0192 {
    UnkResPathVec first;
    UnkResPathVec second;

    ~UnkAudioPresetPair_b0192() { fn_71010396a8(this); }
};

struct UnkAudioPresetSet_b0192 {
    UnkAudioPresetPair_b0192 items[16];
};

void fn_710103cab0(UnkAudioPresetSet_b0192& set, base::UnkCollectionIter<UnkAudioPresetPair_b0192, u64>* out) {
    *out = base::UnkCollectionIter<UnkAudioPresetPair_b0192, u64>(16, set.items + 16);
}
