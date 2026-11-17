// build-flags: no-strict-aliasing
// sound::CSoundManager unit (tools/tu_map.py, unit 2646): moves an owning pointer into an array of them
// at the given index, growing the array to reach it and freeing the object the place held.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/sound/CSoundManager.h"
#include "types.h"

struct UnkSoundOwned_b0038;

// An owning pointer: moved by taking the other's object (an assignment destroys the old one);
// cleared, then its object destroyed (fn_710102796c) and freed.
struct UnkSoundOwnedPtr_l02b2a8 {
    UnkSoundOwned_b0038* ptr;

    UnkSoundOwnedPtr_l02b2a8() : ptr(nullptr) {}
    UnkSoundOwnedPtr_l02b2a8(UnkSoundOwnedPtr_l02b2a8&& other) {
        UnkSoundOwned_b0038* p = other.ptr;
        other.ptr = nullptr;
        ptr = p;
    }
    UnkSoundOwnedPtr_l02b2a8& operator=(UnkSoundOwnedPtr_l02b2a8&& other) {
        UnkSoundOwned_b0038* p = other.ptr;
        other.ptr = nullptr;
        UnkSoundOwned_b0038* old = ptr;
        ptr = p;
        if (old) {
            fn_710102796c(old);
            fn_7100083c00(old);
        }
        return *this;
    }
    ~UnkSoundOwnedPtr_l02b2a8() {
        UnkSoundOwned_b0038* p = ptr;
        ptr = nullptr;
        if (p) {
            fn_710102796c(p);
            fn_7100083c00(p);
        }
    }
};

using UnkSoundOwnedArray_l02b2a8 = base::global::CRntVector<UnkSoundOwnedPtr_l02b2a8>;

// Allocates new_capacity entries, moves the entries over and frees the old array.
static inline void Reallocate(UnkSoundOwnedArray_l02b2a8* v, u32 new_capacity) {
    UnkSoundOwnedPtr_l02b2a8* p = static_cast<UnkSoundOwnedPtr_l02b2a8*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkSoundOwnedPtr_l02b2a8), 8, v->flags));
    if (v->data) {
        UnkSoundOwnedPtr_l02b2a8* src = v->data;
        UnkSoundOwnedPtr_l02b2a8* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkSoundOwnedPtr_l02b2a8(static_cast<UnkSoundOwnedPtr_l02b2a8&&>(*src));
            src->~UnkSoundOwnedPtr_l02b2a8();
        }
        v->allocator->vf_0x48(v->data);
    }
    v->data = p;
    v->capacity = new_capacity;
}

void fn_710102b2a8(UnkSoundOwnedArray_l02b2a8* v, const u32* index, UnkSoundOwnedPtr_l02b2a8* value) {
    u32 i = *index;
    // Past the end: grow to the next multiple of the step, then to exactly i + 1 empty entries.
    if (v->field_0x14 && v->count <= i) {
        u32 cap = v->capacity;
        if (cap <= i && v->step != 0) {
            u32 grown = (i / v->step + 1) * v->step;
            if (cap < grown) {
                Reallocate(v, grown);
                cap = grown;
            }
        }
        u32 n = i + 1;
        if (cap < n)
            Reallocate(v, n);
        for (u32 j = v->count; j < n; ++j)
            new (&v->data[j]) UnkSoundOwnedPtr_l02b2a8();
        v->count = n;
    }
    v->data[i] = static_cast<UnkSoundOwnedPtr_l02b2a8&&>(*value);
}
