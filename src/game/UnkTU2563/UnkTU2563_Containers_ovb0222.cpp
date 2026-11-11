// CMinimapData's unit (tools/tu_map.py unit 2563, no reliable name): containers of the minimap data.
// A clear of an array of list pairs, the layer built beside a keyed insert, a record copy-assignment, an erase
// of a list pair, and a keyed insert of a default item.
// Names and layouts are placeholders inferred from the code (siblings: src/game/CMinimapData_3.cpp,
// src/game/CMinimapData/).
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU83.h"
#include "game_s2/UnkItem50.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id built empty and released like a destructor would, but in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId_ovb0222 {
    UnkStringData* data;

    __attribute__((always_inline)) UnkReleasedStrId_ovb0222() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    __attribute__((always_inline)) ~UnkReleasedStrId_ovb0222() {
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

// Two arrays that free their storage when destroyed, and an empty 2D bound (0x60 bytes).
struct UnkListPair_ovb0222 {
    base::UnkVector<void*> list0{0xf};
    base::UnkVector<void*> list1{0xf};
    f32 min[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max[2] = {-__FLT_MAX__, -__FLT_MAX__};
    ~UnkListPair_ovb0222() {
        list1.Free();
        list0.Free();
    }
};
static_assert(sizeof(UnkListPair_ovb0222) == 0x60, "layout");

// Frees the pairs' arrays and the array itself.
void fn_7100e52958(base::UnkVector<UnkListPair_ovb0222>* self) { self->Free(); }

// Three arrays, two flags and two empty 2D bounds (0xa8 bytes), built value-initialised.
struct UnkLayer_ovb0222 {
    base::UnkVector<void*> list0{7};
    u32 field_0x28 = 0;
    bool field_0x2c = false;
    bool field_0x2d = true;
    f32 min0[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max0[2] = {-__FLT_MAX__, -__FLT_MAX__};
    u32 field_0x40 = 0;
    base::UnkVector<void*> list1{0xf};
    base::UnkVector<void*> list2{0xf};
    f32 min1[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max1[2] = {-__FLT_MAX__, -__FLT_MAX__};
    ~UnkLayer_ovb0222() {
        list2.Free();
        list1.Free();
        list0.Free();
    }
};
static_assert(sizeof(UnkLayer_ovb0222) == 0xa8, "layout");

// The keyed layer insert (src/game/UnkTU2563/UnkTU2563_Inserts_ovb0222.cpp).
template <class Item>
struct UnkKeyedItems_ovb0222;
struct UnkLayerMove_ovb0222;
UnkLayerMove_ovb0222* fn_7100e5fa4c(UnkKeyedItems_ovb0222<UnkLayerMove_ovb0222>* self, const CStrId* key,
                                    UnkLayerMove_ovb0222* item);

void* fn_7100e45040(void* self, void* key) {
    UnkLayer_ovb0222 layer = UnkLayer_ovb0222();
    return fn_7100e5fa4c(static_cast<UnkKeyedItems_ovb0222<UnkLayerMove_ovb0222>*>(self),
                         static_cast<const CStrId*>(key), reinterpret_cast<UnkLayerMove_ovb0222*>(&layer));
}

// A record of ten words, two string ids and a trailing array copied out of line (fn_71000ce19c).
struct UnkStruct_0x28;
struct UnkIdRecord_ovb0222 {
    u32 f[10];
    CStrId a;
    CStrId b;
    u8 rest[0x20];

    __attribute__((always_inline)) UnkIdRecord_ovb0222& operator=(const UnkIdRecord_ovb0222& o) {
        f[0] = o.f[0];
        f[1] = o.f[1];
        f[2] = o.f[2];
        f[3] = o.f[3];
        f[4] = o.f[4];
        f[5] = o.f[5];
        f[6] = o.f[6];
        f[7] = o.f[7];
        f[8] = o.f[8];
        f[9] = o.f[9];
        a = o.a;
        b = o.b;
        fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(rest), reinterpret_cast<const UnkStruct_0x28*>(o.rest));
        return *this;
    }
};

__attribute__((flatten)) void* fn_7100e4feb8(void* self, const void* other) {
    return &(*static_cast<UnkIdRecord_ovb0222*>(self) = *static_cast<const UnkIdRecord_ovb0222*>(other));
}

// Erases the list pair at the index from an array of them (UnkVector::EraseAt; the moved-from pair is destroyed
// again, its storage reloaded through the array).
struct UnkCollection_Minimap_A6_b67;
void fn_7100e53520(UnkCollection_Minimap_A6_b67* self_, u32 index) {
    auto* self = reinterpret_cast<base::UnkVector<UnkFreedVectorPair>*>(self_);
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkFreedVectorPair();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkFreedVectorPair* cur = &self->data[i];
            new (cur) UnkFreedVectorPair(static_cast<UnkFreedVectorPair&&>(cur[1]));
            (&self->data[i])[1].~UnkFreedVectorPair();
        }
        self->count = self->count - 1;
    }
}

// An element of the keyed arrays (0x80 bytes): four words, a string id, two arrays and an empty 2D bound,
// built value-initialised on the stack and inserted under a key (the inserts of src/game/CMinimapData_3.cpp).
struct UnkKeyedItem_ovb0222 {
    u32 field_0x0 = 0, field_0x4 = 0, field_0x8 = 0, field_0xc = 0;
    UnkReleasedStrId_ovb0222 name;
    base::UnkVector<void*> list0{0xf};
    base::UnkVector<void*> list1{0xf};
    f32 min[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max[2] = {-__FLT_MAX__, -__FLT_MAX__};
    u32 field_0x78;
    ~UnkKeyedItem_ovb0222() {
        list1.Free();
        list0.Free();
    }
};
static_assert(sizeof(UnkKeyedItem_ovb0222) == 0x80, "layout");

template <class Item, class Key = base::global::CStrId>
struct UnkMinimapKeyedItems;
struct UnkMinimapKeyedItem;
UnkMinimapKeyedItem* fn_7100e614d0(UnkMinimapKeyedItems<UnkMinimapKeyedItem>* self, const base::global::CStrId* key,
                                   UnkMinimapKeyedItem* item);

void* fn_7100e453f8(void* self, const void* key) {
    UnkKeyedItem_ovb0222 item = UnkKeyedItem_ovb0222();
    return fn_7100e614d0(static_cast<UnkMinimapKeyedItems<UnkMinimapKeyedItem>*>(self),
                         static_cast<const base::global::CStrId*>(key),
                         reinterpret_cast<UnkMinimapKeyedItem*>(&item));
}
