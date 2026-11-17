// CTriggerComponent unit (tools/tu_map.py): moves an owned object (destroyed by fn_7100a632e8) into an
// array of owning pointers at the given index, shifting the later ones up; the array grows first when it
// is full. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CTriggerComponent.h"
#include "types.h"

using UnkTriggerOwnedPtr_a64098 = UnkTriggerOwned_b56*;

// Moves the owning pointer in `from` to `to`, leaving `from` empty.
static inline void MoveOwned(UnkTriggerOwnedPtr_a64098* to, UnkTriggerOwnedPtr_a64098* from) {
    UnkTriggerOwned_b56* moved = *from;
    *from = nullptr;
    *to = moved;
}

// Destroys and frees the object `slot` owns, leaving it empty.
static inline void DestroyOwned(UnkTriggerOwnedPtr_a64098* slot) {
    UnkTriggerOwned_b56* old = *slot;
    *slot = nullptr;
    if (old) {
        fn_7100a632e8(old);
        operator delete(old);
    }
}

// Allocates new_capacity slots and moves the pointers over, freeing the old array.
static inline UnkTriggerOwnedPtr_a64098* MoveTo(base::global::CRntVector<UnkTriggerOwnedPtr_a64098>* v,
                                                u32 new_capacity) {
    UnkTriggerOwnedPtr_a64098* p = static_cast<UnkTriggerOwnedPtr_a64098*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkTriggerOwnedPtr_a64098), 8, v->flags));
    if (v->data) {
        UnkTriggerOwnedPtr_a64098* src = v->data;
        UnkTriggerOwnedPtr_a64098* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            MoveOwned(dst, src);
            DestroyOwned(src);
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100a64098(base::global::CRntVector<UnkTriggerOwnedPtr_a64098>* v, UnkTriggerOwnedPtr_a64098* value,
                   u32 index) {
    using Entry = UnkTriggerOwnedPtr_a64098;
    u32 cap = v->capacity;
    u32 n = v->count;
    Entry* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        if ((s32)new_capacity <= (s32)index)
            for (;;) {
            }
        if (cap < new_capacity) {
            p = MoveTo(v, new_capacity);
            index = v->count;
            v->data = p;
            v->capacity = new_capacity;
        } else {
            p = v->data;
        }
    } else {
        if (cap <= n && v->step != 0) {
            u32 grown = (n / v->step + 1) * v->step;
            if (cap < grown) {
                Entry* q = MoveTo(v, grown);
                v->data = q;
                v->capacity = grown;
                n = v->count;
            }
        }
        // The pointers from the last down to the index move up one place.
        for (u32 i = n - 1; i >= index; --i) {
            Entry* q = &v->data[i];
            MoveOwned(q + 1, q);
            DestroyOwned(&v->data[i]);
            if (i == 0)
                break;
        }
        p = v->data;
    }
    MoveOwned(&p[index], value);
    ++v->count;
}
