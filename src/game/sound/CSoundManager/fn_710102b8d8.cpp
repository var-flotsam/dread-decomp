// sound::CSoundManager unit (tools/tu_map.py, unit 2646): moves an owning pointer into an array of them
// at the given index, shifting the later ones up. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

struct UnkSoundOwned_b0038;

// An owning pointer: moved by taking the other's object; cleared, then its object destroyed
// (fn_710102796c) and freed.
struct UnkSoundOwnedPtr_l02b8d8 {
    UnkSoundOwned_b0038* ptr;

    UnkSoundOwnedPtr_l02b8d8(UnkSoundOwnedPtr_l02b8d8&& other) {
        UnkSoundOwned_b0038* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    ~UnkSoundOwnedPtr_l02b8d8() {
        UnkSoundOwned_b0038* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_710102796c(p);
            fn_7100083c00(p);
        }
    }
};

using UnkSoundOwnedArray_l02b8d8 = base::global::CRntVector<UnkSoundOwnedPtr_l02b8d8>;

// Allocates new_capacity entries and moves the entries over, freeing the old array.
static inline UnkSoundOwnedPtr_l02b8d8* MoveTo(UnkSoundOwnedArray_l02b8d8* v, u32 new_capacity) {
    UnkSoundOwnedPtr_l02b8d8* p = static_cast<UnkSoundOwnedPtr_l02b8d8*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkSoundOwnedPtr_l02b8d8), 8, v->flags));
    if (v->data) {
        UnkSoundOwnedPtr_l02b8d8* src = v->data;
        UnkSoundOwnedPtr_l02b8d8* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkSoundOwnedPtr_l02b8d8(static_cast<UnkSoundOwnedPtr_l02b8d8&&>(*src));
            src->~UnkSoundOwnedPtr_l02b8d8();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

void fn_710102b8d8(UnkSoundOwnedArray_l02b8d8* v, UnkSoundOwnedPtr_l02b8d8&& value, u32 index) {
    using Entry = UnkSoundOwnedPtr_l02b8d8;
    u32 cap = v->capacity;
    u32 n = v->count;
    Entry* p;
    if (n == index) {
        u32 new_capacity = cap;
        if (cap <= n && v->step != 0)
            new_capacity = (index / v->step + 1) * v->step;
        // The original hangs here when the index does not fit.
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
            v->data[i].~Entry();
            if (i == 0)
                break;
        }
        p = v->data;
    }
    new (&p[index]) Entry(static_cast<Entry&&>(value));
    ++v->count;
}
