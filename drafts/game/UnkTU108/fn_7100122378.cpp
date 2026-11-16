// Unit 108: moves an 8-byte pair into a growable array at the given index, shifting the elements
// after it up. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

// An 8-byte element of two words, moved word by word.
struct UnkPair_l122378 {
    u32 value[2];

    UnkPair_l122378(UnkPair_l122378&& other) {
        value[0] = other.value[0];
        value[1] = other.value[1];
    }
};

using UnkPairVector_l122378 = base::global::CRntVector<UnkPair_l122378>;

// Allocates new_capacity elements and moves the elements over, freeing the old array.
static inline UnkPair_l122378* MoveTo(UnkPairVector_l122378* v, u32 new_capacity) {
    UnkPair_l122378* p = static_cast<UnkPair_l122378*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkPair_l122378), 8, v->flags));
    if (v->data) {
        UnkPair_l122378* src = v->data;
        UnkPair_l122378* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src)
            new (dst) UnkPair_l122378(static_cast<UnkPair_l122378&&>(*src));
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_7100122378(UnkPairVector_l122378* v, UnkPair_l122378* value, u32 index) {
    using Entry = UnkPair_l122378;
    u32 cap = v->capacity;
    u32 n = v->count;
    Entry* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        // Appending past the largest signed capacity hangs, as the original does.
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
        for (u32 i = n - 1; i >= index; --i) {
            Entry* q = &v->data[i];
            new (q + 1) Entry(static_cast<Entry&&>(*q));
            if (i == 0)
                break;
        }
        p = v->data;
    }
    new (&p[index]) Entry(static_cast<Entry&&>(*value));
    ++v->count;
}
