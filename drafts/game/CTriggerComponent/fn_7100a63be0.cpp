// CTriggerComponent unit (tools/tu_map.py): the set_move slot of the reflected array of owned activation
// conditions (destroyed by fn_7100a632e8): grows the array up to the index with empty slots, then moves
// the given pointer into that slot, destroying what it held. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CTriggerComponent.h"
#include "types.h"

using UnkTriggerOwnedPtr_a63be0 = UnkTriggerOwned_b56*;
using UnkTriggerOwnedVec_a63be0 = base::global::CRntVector<UnkTriggerOwnedPtr_a63be0>;

// Destroys and frees the object `slot` owns, leaving it empty.
static inline void DestroyOwned(UnkTriggerOwnedPtr_a63be0* slot) {
    UnkTriggerOwned_b56* old = *slot;
    *slot = nullptr;
    if (old) {
        fn_7100a632e8(old);
        operator delete(old);
    }
}

// Moves the pointers into a new array of `n` and frees the old one.
static inline void Reallocate(UnkTriggerOwnedVec_a63be0* self, u32 n) {
    auto* p = static_cast<UnkTriggerOwnedPtr_a63be0*>(
        self->allocator->vf_0x40((u64)n * sizeof(UnkTriggerOwnedPtr_a63be0), 8, self->flags));
    if (self->data) {
        UnkTriggerOwnedPtr_a63be0* src = self->data;
        UnkTriggerOwnedPtr_a63be0* dst = p;
        for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
            UnkTriggerOwned_b56* moved = *src;
            *src = nullptr;
            *dst = moved;
            DestroyOwned(src);
        }
        self->allocator->vf_0x48(self->data);
    }
    self->data = p;
    self->capacity = n;
}

void fn_7100a63be0(UnkTriggerOwnedVec_a63be0* self, const u32* index_ptr, UnkTriggerOwnedPtr_a63be0* value) {
    u32 index = *index_ptr;
    if (self->field_0x14 && self->count <= index) {
        // First to the next multiple of the step, then to exactly index + 1 slots.
        u32 cap = self->capacity;
        if (cap <= index && self->step != 0) {
            u32 grown = (index / self->step + 1) * self->step;
            if (cap < grown) {
                Reallocate(self, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(self, n);
        for (u32 i = self->count; i < n; ++i)
            self->data[i] = nullptr;
        self->count = n;
    }
    UnkTriggerOwnedPtr_a63be0* slot = &self->data[index];
    UnkTriggerOwned_b56* moved = *value;
    *value = nullptr;
    UnkTriggerOwned_b56* old = *slot;
    *slot = moved;
    if (old) {
        fn_7100a632e8(old);
        operator delete(old);
    }
}
