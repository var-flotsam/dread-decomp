// CNavMeshItemComponent unit: a reflected flat map's "set by key" slots (CCollectionType +0x148 move, +0x140
// copy; src/reflection_types/collection_types_00.cpp), copies of the CBreakableTileGroupComponent pair
// (src/game/CBreakableTileGroupComponent/UnkReflFlatMapMoveSet_A5_b71.cpp) with 8-byte elements in the copied
// arrays. Keys are 8-byte words, values 0x28-byte growable arrays. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkTileArray_A4_b72 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    void* allocator;

    __attribute__((always_inline)) void MoveFrom(UnkTileArray_A4_b72* o) {
        if (this == o)
            return;
        count = 0;
        allocator = o->allocator;
        flags = o->flags;
        step = o->step;
        growable = o->growable;
        count = o->count;
        capacity = o->capacity;
        data = o->data;
        o->data = nullptr;
        o->count = 0;
        o->capacity = 0;
    }
};
static_assert(sizeof(UnkTileArray_A4_b72) == 0x28, "layout");

struct UnkTileFlatMap_A4_b72 {
    UnkTileArray_A4_b72* values;
    u64* keys;
    u32 count;

    UnkTileArray_A4_b72* End() { return &values[count]; }
    UnkTileArray_A4_b72* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_71009c3c94(UnkTileFlatMap_A4_b72* self, const u64* key, UnkTileArray_A4_b72* value) {
    UnkTileArray_A4_b72* dst = self->Find(key);
    if (dst != self->End())
        dst->MoveFrom(value);
}

// The copy slot (+0x140) of the same map, over the engine array of 8-byte elements: the entry's elements are
// dropped, its array grown to the given array's capacity when smaller, and the elements copied in.
struct UnkTileElem_A4_b72 {
    u64 a;
};
using UnkTileVector_A4_b72 = base::UnkVector<UnkTileElem_A4_b72>;

struct UnkTileVecFlatMap_A4_b72 {
    UnkTileVector_A4_b72* values;
    u64* keys;
    u32 count;

    UnkTileVector_A4_b72* End() { return &values[count]; }
    UnkTileVector_A4_b72* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

static inline __attribute__((always_inline)) void CopyFrom_A4_b72(UnkTileVector_A4_b72* self,
                                                                  const UnkTileVector_A4_b72* o) {
    if (self == o)
        return;
    self->Clear();
    self->Reserve(o->capacity);
    __builtin_memcpy(self->data, o->data, (u64)o->count * sizeof(UnkTileElem_A4_b72));
    self->count = o->count;
}

void fn_71009c3b78(UnkTileVecFlatMap_A4_b72* self, const u64* key, const UnkTileVector_A4_b72* value) {
    UnkTileVector_A4_b72* dst = self->Find(key);
    if (dst != self->End())
        CopyFrom_A4_b72(dst, value);
}
