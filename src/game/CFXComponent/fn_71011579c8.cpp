// CFXComponent's unit: grows the effect list's parallel arrays (0x20-byte entries at +0x0, string-id names
// at +0x8, count +0x10, capacity +0x14, allocator +0x20) to `capacity`: the entries are copied over, the
// names moved over and released. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkFXName_l1579c8 {
    UnkStringData* data;

    UnkFXName_l1579c8(UnkFXName_l1579c8&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkFXName_l1579c8() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkFXEntry_l1579c8 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
};

struct UnkFXList_l1579c8 {
    UnkFXEntry_l1579c8* values;  // 0x0
    UnkFXName_l1579c8* names;    // 0x8
    u32 count;                   // 0x10
    u32 capacity;                // 0x14
    u32 step;                    // 0x18
    u32 flags;                   // 0x1c
    base::UnkAllocator* allocator;  // 0x20
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_71011579c8(UnkFXList_l1579c8* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkFXEntry_l1579c8* values = static_cast<UnkFXEntry_l1579c8*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkFXEntry_l1579c8), 8, self->flags));
    UnkFXName_l1579c8* names = static_cast<UnkFXName_l1579c8*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkFXName_l1579c8), 8, self->flags));
    if (self->values) {
        UnkFXEntry_l1579c8* src = self->values;
        UnkFXEntry_l1579c8* dst = values;
        for (u32 i = 0; i < self->count; ++i)
            *dst++ = *src++;
        self->allocator->vf_0x48(self->values);
    }
    if (self->names) {
        UnkFXName_l1579c8* src = self->names;
        u64 j = 0;
        for (u32 i = 0; i < self->count; ++i, ++j, ++src) {
            new (&names[j]) UnkFXName_l1579c8(static_cast<UnkFXName_l1579c8&&>(*src));
            src->~UnkFXName_l1579c8();
        }
        self->allocator->vf_0x48(self->names);
    }
    self->values = values;
    self->names = names;
    self->capacity = capacity;
}
