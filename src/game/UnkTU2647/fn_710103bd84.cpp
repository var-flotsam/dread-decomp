// build-flags: no-strict-aliasing
// Unit 2647 (sound::CAudioPreset): the copy assignment of a reflected iterator value over the bank of
// presets (two lists of file-path ids each). Names and layouts are placeholders.
#include "base/UnkCollectionIter.h"
#include "base/UnkResPathVec.h"
#include "types.h"


struct UnkAudioPresetPair_b0192_3bd84 {
    UnkResPathVec first;
    UnkResPathVec second;

    UnkAudioPresetPair_b0192_3bd84& operator=(const UnkAudioPresetPair_b0192_3bd84& other) {
        fn_71000d0958(&first, &other.first);
        fn_71000d0958(&second, &other.second);
        return *this;
    }
};

// The cached (index, element) value: empty (0), owned (1) or referring to an iterator's (2).
struct UnkPresetIterValue_b0192 {
    union {
        struct {
            u64 index;
            UnkAudioPresetPair_b0192_3bd84 element;
        } value;
        struct {
            u64* index;
            UnkAudioPresetPair_b0192_3bd84* element;
        } ref;
    };
    u32 state;  // 0x58

    u64& Index() { return state == 2 ? *ref.index : value.index; }
    UnkAudioPresetPair_b0192_3bd84& Element() { return state == 2 ? *ref.element : value.element; }
};

UnkPresetIterValue_b0192* fn_710103bd84(UnkPresetIterValue_b0192* self, UnkPresetIterValue_b0192* other) {
    if (self == other)
        return self;
    self->Index() = other->Index();
    UnkAudioPresetPair_b0192_3bd84& src = other->Element();
    UnkAudioPresetPair_b0192_3bd84& dst = self->Element();
    dst = src;
    return self;
}
