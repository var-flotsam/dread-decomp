// Unit 81 of tools/tu_map.py (base::reflection::CType): appends a pointer under an 8-byte key to the
// keyed array an object holds at +0xb8 (values +0xb8, keys +0xc0, count +0xc8, capacity +0xcc, step
// +0xd0, allocation flags +0xd4, allocator +0xd8), growing both arrays by the step when full (the values
// copied as a block, the keys one by one). Near fn_7100dd0b9c (src/game/CBossRushManager_KeyedWords_b49.cpp).
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

struct UnkKeyedPtrs_b52 {
    void** values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Grow(u32 new_capacity) {
        if (count >= new_capacity)
            return;
        void** new_values = static_cast<void**>(allocator->vf_0x40((u64)new_capacity * sizeof(void*), 8, flags));
        u64* new_keys = static_cast<u64*>(allocator->vf_0x40((u64)new_capacity * sizeof(u64), 8, flags));
        if (values) {
            memcpy(new_values, values, (u64)count * sizeof(void*));
            allocator->vf_0x48(values);
        }
        if (u64* src = keys) {
            // Copied bytewise: the original rereads the count after each key.
            u64 j = 0;
            for (u32 i = 0; i < count; ++i, ++j)
                __builtin_memcpy(&new_keys[j], &src[j], sizeof(u64));
            allocator->vf_0x48(keys);
        }
        values = new_values;
        keys = new_keys;
        capacity = new_capacity;
    }
};

struct UnkKeyedPtrsOwner_b52 {
    u8 pad_0x0[0xb8];
    UnkKeyedPtrs_b52 entries;
};

void fn_710009dba0(UnkKeyedPtrsOwner_b52* self, const u64* key, void* value) {
    UnkKeyedPtrs_b52& a = self->entries;
    if (a.count == a.capacity)
        a.Grow(a.step + a.count);
    // Both stored bytewise: the original rereads the arrays and the count after each.
    __builtin_memcpy(&a.keys[a.count], key, sizeof(u64));
    __builtin_memcpy(&a.values[a.count], &value, sizeof(void*));
    ++a.count;
}
