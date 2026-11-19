// Unit 2647 (sound::CAudioPreset): replaces one preset of a bank of sixteen with an empty one.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPresetList_b0192 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Init() {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        field_0x14 = true;
        flags = 0xf;
        allocator = fn_7100083990();
    }
};

struct UnkAudioPresetPair_b0192 {
    UnkPresetList_b0192 first;
    UnkPresetList_b0192 second;
    UnkAudioPresetPair_b0192() {
        __builtin_memset(this, 0, sizeof(*this));
        first.Init();
        second.Init();
    }
};

struct UnkAudioPresetSet_b0192 {
    UnkAudioPresetPair_b0192 items[16];
};

void fn_71010396a8(UnkAudioPresetPair_b0192* self);
UnkAudioPresetPair_b0192* fn_710103a414(UnkAudioPresetPair_b0192* self, UnkAudioPresetPair_b0192* other);

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_710103c338(UnkAudioPresetSet_b0192* self, const u32* index) {
    UnkAudioPresetPair_b0192* slot = &self->items[*index];
    UnkAudioPresetPair_b0192 empty;
    fn_710103a414(slot, &empty);
    fn_71010396a8(&empty);
}
