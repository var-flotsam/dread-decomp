// CMinimapData's unit (tools/tu_map.py unit 2563, no reliable name): appends to keyed arrays (parallel item
// and key arrays; the key is stored bitwise and retained). A copy of a string-id vector, and a layer moved in.
// Names and layouts are placeholders inferred from the code (sibling: src/game/CMinimapData_3.cpp).
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkFreedVectorPair.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// Copies a key into its slot, bitwise (the original reloads the count after the store) and retains it.
static inline void CopyKey(CStrId* slot, const CStrId* key) {
    UnkStringData* data = key->data;
    __builtin_memcpy(slot, &data, sizeof(data));
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

template <class Item>
struct UnkKeyedItems_ovb0222 {
    Item* items;
    CStrId* keys;  // +0x8
    u32 count;     // +0x10
    u32 capacity;
    u32 step;

    // Appends a copy of `item` under `key`.
    __attribute__((always_inline)) void Add(void (*grow)(void*, u32), const CStrId* key, const Item* item) {
        if (count == capacity)
            grow(this, step + count);
        CopyKey(&keys[count], key);
        new (&items[count]) Item(*item);
        ++count;
    }
    // Appends `item` (moved) under `key`; returns the new item.
    __attribute__((always_inline)) Item* AddMove(void (*grow)(void*, u32), const CStrId* key, Item* item) {
        if (count == capacity)
            grow(this, step + count);
        CopyKey(&keys[count], key);
        new (&items[count]) Item(static_cast<Item&&>(*item));
        u32 n = ++count;
        return n != 0 ? &items[n] - 1 : items;
    }
};

void fn_71009ba838(void*, u32);

void fn_7100e5d184(UnkKeyedItems_ovb0222<base::UnkStrIdVec>* self, const CStrId* key, const base::UnkStrIdVec* item) {
    self->Add(fn_71009ba838, key, item);
}

// A layer (0xa8 bytes: three arrays, two flags and two empty 2D bounds) moved in: the arrays are taken, the
// second flag is raised and the first bound is reset.
struct UnkLayerMove_ovb0222 {
    UnkFreedVector<void*> list0;
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    f32 min0[2];
    f32 max0[2];
    u32 field_0x40;
    UnkFreedVector<void*> list1;
    UnkFreedVector<void*> list2;
    u32 field_0x98, field_0x9c, field_0xa0, field_0xa4;

    UnkLayerMove_ovb0222(UnkLayerMove_ovb0222&& o)
        : list0(static_cast<UnkFreedVector<void*>&&>(o.list0)), field_0x28(o.field_0x28),
          field_0x2c(o.field_0x2c), field_0x2d(true), min0{__FLT_MAX__, __FLT_MAX__},
          max0{-__FLT_MAX__, -__FLT_MAX__}, field_0x40(o.field_0x40),
          list1(static_cast<UnkFreedVector<void*>&&>(o.list1)),
          list2(static_cast<UnkFreedVector<void*>&&>(o.list2)), field_0x98(o.field_0x98),
          field_0x9c(o.field_0x9c), field_0xa0(o.field_0xa0), field_0xa4(o.field_0xa4) {}
};
static_assert(sizeof(UnkLayerMove_ovb0222) == 0xa8, "layout");

void fn_7100e5f348(void*, u32);

UnkLayerMove_ovb0222* fn_7100e5fa4c(UnkKeyedItems_ovb0222<UnkLayerMove_ovb0222>* self, const CStrId* key,
                                    UnkLayerMove_ovb0222* item) {
    return self->AddMove(fn_7100e5f348, key, item);
}
