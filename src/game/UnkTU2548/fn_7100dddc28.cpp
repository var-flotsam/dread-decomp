// build-flags: no-strict-aliasing
// Unit 2548 of tools/tu_map.py: the element at an index of an array of 0x30-byte polymorphic
// holders (vtable g_unk_7101cac988), growing the array to reach it (CRntVector::GrowAt, an
// out-of-line copy of it written out).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2661.h"
#include "types.h"

// The type the callers declare the result with (src/game/CCutscene/CTunableCutscene_CallableCopy.cpp).
struct UnkCutsceneHolder;

struct UnkHolder_ldddc28;

// The holder's vtable as used here: its first slot destroys it.
struct UnkHolderVtable_ldddc28 {
    void (*destroy)(UnkHolder_ldddc28* self);
};

// 27 bytes of the holder's data, copied as a block.
struct UnkHolderPayload_ldddc28 {
    u8 bytes[27];
};

struct UnkHolder_ldddc28 {
    const UnkHolderVtable_ldddc28* vtable;
    u64 field_0x8;
    UnkHolderPayload_ldddc28 payload;  // 0x10

    UnkHolder_ldddc28() : vtable(reinterpret_cast<const UnkHolderVtable_ldddc28*>(g_unk_7101cac988 + 2)) {}
    UnkHolder_ldddc28(const UnkHolder_ldddc28& other)
        : vtable(reinterpret_cast<const UnkHolderVtable_ldddc28*>(g_unk_7101cac988 + 2)),
          field_0x8(other.field_0x8),
          payload(other.payload) {}
    // The virtual destructor.
    ~UnkHolder_ldddc28() { vtable->destroy(this); }
};
static_assert(sizeof(UnkHolder_ldddc28) == 0x30, "layout");

using UnkHolders_ldddc28 = base::global::CRntVector<UnkHolder_ldddc28>;

// Allocates new_capacity holders and moves the holders over, freeing the old array.
static inline UnkHolder_ldddc28* MoveTo(UnkHolders_ldddc28* v, u32 new_capacity) {
    UnkHolder_ldddc28* p = static_cast<UnkHolder_ldddc28*>(
        v->allocator->vf_0x40((u64)new_capacity * sizeof(UnkHolder_ldddc28), 8, v->flags));
    if (v->data) {
        UnkHolder_ldddc28* src = v->data;
        UnkHolder_ldddc28* dst = p;
        for (u32 i = 0; i < v->count; ++i, ++dst, ++src) {
            new (dst) UnkHolder_ldddc28(*src);
            src->~UnkHolder_ldddc28();
        }
        v->allocator->vf_0x48(v->data);
    }
    return p;
}

static inline void Reallocate(UnkHolders_ldddc28* v, u32 new_capacity) {
    UnkHolder_ldddc28* p = MoveTo(v, new_capacity);
    v->data = p;
    v->capacity = new_capacity;
}

UnkCutsceneHolder* fn_7100dddc28(void* alloc, u32 index) {
    UnkHolders_ldddc28* v = static_cast<UnkHolders_ldddc28*>(alloc);
    if (v->field_0x14 && v->count <= index) {
        u32 cap = v->capacity;
        if (cap <= index && v->step != 0) {
            u32 grown = (index / v->step + 1) * v->step;
            if (cap < grown) {
                Reallocate(v, grown);
                cap = grown;
            }
        }
        u32 n = index + 1;
        if (cap < n)
            Reallocate(v, n);
        for (u32 i = v->count; i < n; ++i)
            new (&v->data[i]) UnkHolder_ldddc28();
        v->count = n;
    }
    return reinterpret_cast<UnkCutsceneHolder*>(&v->data[index]);
}
