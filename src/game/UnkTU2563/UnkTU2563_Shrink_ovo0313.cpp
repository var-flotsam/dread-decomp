// Unit 2563 (tools/tu_map.py; hint CMinimapData): the shrink-to-count of two keyed containers, parallel
// arrays of items and string ids: one of 0x28-byte records (fn_7100e5bdf0) and one of arrays
// (fn_7100e5dd0c, declared in UnkTU2563_Records_ovb0223.cpp). Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id whose destructor decrements and tests in two steps (the original reloads the id after the
// decrement). Its pointer is stored bytewise, so the destructor of the moved-from id reloads the pool.
struct UnkMinimapId_o0313 {
    UnkStringData* data;

    UnkMinimapId_o0313(UnkMinimapId_o0313&& other) {
        UnkStringData* d = other.data;
        __builtin_memcpy(&data, &d, sizeof(d));
        // Keeps the pool pointer loaded after the copy, as the original loads it for each id.
        asm volatile("" ::: "memory");
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkMinimapId_o0313() {
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

// An item of the keyed arrays (0x28 bytes): six plain words, a string id and a 16-bit value.
struct UnkBoundsItem_o0313 {
    u32 field_0x0, field_0x4, field_0x8, field_0xc, field_0x10, field_0x14;
    UnkMinimapId_o0313 name;  // 0x18
    u16 field_0x20;

    UnkBoundsItem_o0313(UnkBoundsItem_o0313&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          name(static_cast<UnkMinimapId_o0313&&>(other.name)) {
        __builtin_memcpy(&field_0x20, &other.field_0x20, sizeof(field_0x20));
    }
};
static_assert(sizeof(UnkBoundsItem_o0313) == 0x28, "layout");

template <class V>
struct UnkKeyedItems_o0313 {
    V* items;
    UnkMinimapId_o0313* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Shrinks both arrays to the count: moves the items and copies the ids (counting a use) before releasing the old ones.
void fn_7100e5bdf0(UnkKeyedItems_o0313<UnkBoundsItem_o0313>* self) {
    using V = UnkBoundsItem_o0313;
    if (self->count == self->capacity)
        return;
    V* items = static_cast<V*>(self->allocator->vf_0x40((u64)self->count * sizeof(V), 8, self->flags));
    auto* keys = static_cast<UnkMinimapId_o0313*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkMinimapId_o0313), 8, self->flags));
    if (V* src = self->items) {
        V* dst = items;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) V(static_cast<V&&>(*src));
            src->~V();
        }
        self->allocator->vf_0x48(self->items);
    }
    if (UnkMinimapId_o0313* src = self->keys) {
        CStrId* dst = reinterpret_cast<CStrId*>(keys);
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) CStrId(*reinterpret_cast<CStrId*>(src));
            reinterpret_cast<CStrId*>(src)->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->items = items;
    self->keys = keys;
    self->capacity = self->count;
}

// ---- An id-keyed map of arrays ----

struct UnkStrIdVecMap_b0223;

struct UnkVecRawB {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
static_assert(sizeof(UnkVecRawB) == 0x28, "layout");

struct UnkIdVecTableB {
    UnkVecRawB* items;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Shrinks the arrays to the count.
void fn_7100e5dd0c(UnkStrIdVecMap_b0223* map) {
    UnkIdVecTableB* self = reinterpret_cast<UnkIdVecTableB*>(map);
    if (self->count == self->capacity)
        return;
    UnkVecRawB* new_items =
        static_cast<UnkVecRawB*>(self->allocator->vf_0x40((u64)self->count * sizeof(UnkVecRawB), 8, self->flags));
    CStrId* new_keys =
        static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * 8, 8, self->flags));
    UnkVecRawB* src = self->items;
    if (src) {
        UnkVecRawB* dst = new_items;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            dst->data = src->data;
            dst->count = src->count;
            dst->capacity = src->capacity;
            dst->step = src->step;
            dst->field_0x14 = src->field_0x14;
            dst->flags = src->flags;
            dst->allocator = src->allocator;
            __builtin_memset(src, 0, 16);
        }
        self->allocator->vf_0x48(self->items);
    }
    if (CStrId* src = self->keys) {
        CStrId* dst = new_keys;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) CStrId(*src);
            src->Reset();
        }
        self->allocator->vf_0x48(self->keys);
    }
    self->items = new_items;
    self->keys = new_keys;
    self->capacity = self->count;
}
