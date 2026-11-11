// The counterpart of fn_71002d2740: removes the entry whose value is the global source's +0x30
// from the parallel arrays at +0xf0 of the object at +0x18 (the last entry is moved into its
// slot). Names are placeholders.
#include "base/UnkPairArrays.h"
#include "game/animtree/CAnimTreeElement.h"

struct UnkValueSource {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkPairData {
    u8 pad_0x0[0xf0];
    base::UnkPairArrays arrays;
};

struct UnkPairOwner {
    u8 pad_0x0[0x18];
    UnkPairData* field_0x18;
};

static inline u64* FindByValue(base::UnkPairArrays* a, u64 value) {
    for (u64 i = 0; i != a->count; ++i) {
        if (a->values[i] == value) {
            return &a->keys[i];
        }
    }
    return a->keys + a->count;
}

static inline void EraseAt(base::UnkPairArrays* a, u32 index) {
    u32 last = a->count - 1;
    if (last != index) {
        __builtin_memcpy(&a->keys[index], &a->keys[last], sizeof(u64));
        __builtin_memcpy(&a->values[index], &a->values[a->count - 1], sizeof(u64));
    }
    a->count = a->count - 1;
}

void fn_71002d288c(UnkPairOwner* self) {
    UnkPairData* d = self->field_0x18;
    base::UnkPairArrays* a = &d->arrays;
    u64* it = FindByValue(a, (*fn_71002dba48(self))->field_0x30);
    if (it == a->keys + a->count) {
        return;
    }
    EraseAt(a, it - a->keys);
}
