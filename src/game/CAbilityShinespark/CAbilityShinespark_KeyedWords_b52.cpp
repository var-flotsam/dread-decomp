// Copy of fn_7100dd0b9c (src/game/CBossRushManager_KeyedWords_b49.cpp) in CAbilityShinespark's unit:
// appends a u32 value under a u32 key to a keyed array, growing both arrays by the step when full.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

struct UnkKeyedWords_b52 {
    u32* values;
    u32* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Grow(u32 new_capacity) {
        if (count >= new_capacity)
            return;
        u32* new_values = static_cast<u32*>(allocator->vf_0x40((u64)new_capacity * sizeof(u32), 8, flags));
        u32* new_keys = static_cast<u32*>(allocator->vf_0x40((u64)new_capacity * sizeof(u32), 8, flags));
        if (values) {
            memcpy(new_values, values, (u64)count * sizeof(u32));
            allocator->vf_0x48(values);
        }
        if (keys) {
            memcpy(new_keys, keys, (u64)count * sizeof(u32));
            allocator->vf_0x48(keys);
        }
        values = new_values;
        keys = new_keys;
        capacity = new_capacity;
    }
};

u32* fn_710033140c(UnkKeyedWords_b52* self, const u32* key, const u32* value) {
    if (self->count == self->capacity)
        self->Grow(self->step + self->count);
    u32* key_slot = &self->keys[self->count];
    *key_slot = *key;
    // Copied as a block (the original reloads the array after it).
    __builtin_memcpy(&self->values[self->count], value, sizeof(u32));
    u32 count = self->count + 1;
    u32* data = self->values;
    self->count = count;
    // The last value (the first when the count wrapped to zero).
    return count ? data + count - 1 : data;
}
