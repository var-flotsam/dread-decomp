#include "game_s2/UnkOwnedList.h"
#include "types.h"

// Owned objects with a float key each, sharing one count (UnkOwnedList's pointers, plus the keys
// array).
struct UnkKeyedObjects_ovb0186 {
    union {
        UnkOwnedPtr* values;
    };
    f32* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows both arrays to `capacity` entries, moving the objects and copying the keys.
void fn_710006981c(UnkKeyedObjects_ovb0186* self, u32 capacity) {
    if (self->capacity < capacity) {
        UnkOwnedPtr* new_values =
            static_cast<UnkOwnedPtr*>(self->allocator->vf_0x40((u64)capacity * sizeof(UnkOwnedPtr), 8, self->flags));
        f32* new_keys = static_cast<f32*>(self->allocator->vf_0x40((u64)capacity * sizeof(f32), 8, self->flags));
        UnkOwnedPtr* old_values = self->values;
        if (old_values) {
            UnkOwnedPtr* src = old_values;
            for (u64 i = 0; self->count > (u32)i; ++i, ++src) {
                UnkOwned* moved = src->ptr;
                src->ptr = nullptr;
                new_values[i].ptr = moved;
                src->~UnkOwnedPtr();
            }
            self->allocator->vf_0x48(self->values);
        }
        if (self->keys) {
            __builtin_memcpy(new_keys, self->keys, (u64)self->count * sizeof(f32));
            self->allocator->vf_0x48(self->keys);
        }
        self->values = new_values;
        self->keys = new_keys;
        self->capacity = capacity;
    }
}
