// Unit 2563 (near CMinimapData): the constructor of a large map data object: a string id, an index
// pool of 6094 free slots, an array of array pairs reserved for 32, twenty growable arrays, one
// array of another kind and an empty bounding box. Names and layouts are placeholders.
#include <new>

#include "base/UnkFreedVectorPair.h"
#include "base/core/CAsset.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101aa9cf8[];  // the object's vtable

// A fixed table of slot indices, all unused (-1) at first.
struct UnkSlotPool_e3e25c {
    static constexpr u32 kSlots = 6094;

    void** vtable;
    s32 slots[kSlots];
    u32 field_0x5f40;
    u32 slot_count;
    u32 field_0x5f48;
    u32 field_0x5f4c;
    s32* table;
    u32 last_slot;

    UnkSlotPool_e3e25c() : vtable(g_unk_7101aa9d38 + 2) {
        last_slot = kSlots - 1;
        field_0x5f48 = 195000;
        field_0x5f4c = 0;
        field_0x5f40 = 195000;
        slot_count = kSlots;
        table = slots;
        __builtin_memset(slots, 0xff, sizeof(slots));
    }
};
static_assert(sizeof(UnkSlotPool_e3e25c) == 0x5f60, "UnkSlotPool_e3e25c layout");

// A growable array with a second array alongside (UnkSecondPtrArray's layout), empty.
struct UnkTwinArray_e3e25c {
    void* data = nullptr;
    void* field_0x8 = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;
    u32 flags = 14;
    base::UnkAllocator* allocator = fn_7100083990();
};
static_assert(sizeof(UnkTwinArray_e3e25c) == 0x28, "UnkTwinArray_e3e25c layout");

struct UnkMapData_e3e25c {
    void** vtable;
    u8 pad_0x8[0x58];
    CStrId field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    UnkSlotPool_e3e25c pool;                    // 0x78
    UnkPairArray pairs;                          // 0x5fd8
    UnkTwinArray_e3e25c arrays[16];              // 0x6000
    base::global::CRntVector<void*> field_0x6280;
    UnkTwinArray_e3e25c more_arrays[4];          // 0x62a8
    f32 min_x, min_y;                            // 0x6348
    f32 max_x, max_y;                            // 0x6350
};
static_assert(__builtin_offsetof(UnkMapData_e3e25c, pairs) == 0x5fd8, "pairs");
static_assert(__builtin_offsetof(UnkMapData_e3e25c, field_0x6280) == 0x6280, "field_0x6280");
static_assert(__builtin_offsetof(UnkMapData_e3e25c, min_x) == 0x6348, "bounds");

void fn_7100e3e25c(UnkMapData_e3e25c* self) {
    fn_7100016ad8(self);
    self->vtable = g_unk_7101aa9cf8 + 2;
    self->field_0x60.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0x60.data = &g_unk_7101d47108->empty;
    self->field_0x68 = 0;
    self->field_0x70 = 0;
    new (&self->pool) UnkSlotPool_e3e25c;

    UnkPairArray& pairs = self->pairs;
    pairs.data = nullptr;
    pairs.count = 0;
    pairs.capacity = 0;
    pairs.step = 10;
    pairs.field_0x14 = true;
    pairs.flags = 0xf;
    pairs.allocator = fn_7100083990();
    fn_7100e52b58(&pairs, 0x20);

    for (UnkTwinArray_e3e25c& array : self->arrays)
        new (&array) UnkTwinArray_e3e25c;
    new (&self->field_0x6280) base::global::CRntVector<void*>(0xf);
    for (UnkTwinArray_e3e25c& array : self->more_arrays)
        new (&array) UnkTwinArray_e3e25c;

    self->min_x = __FLT_MAX__;
    self->min_y = __FLT_MAX__;
    self->max_x = -__FLT_MAX__;
    self->max_y = -__FLT_MAX__;
}
