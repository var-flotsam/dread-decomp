// CBreakableTileGroupComponent unit (unit 1167): a reflected flat map's "set by key" move slot (CCollectionType
// +0x148; src/reflection_types/collection_types_00.cpp). Keys are 8-byte words, values 0x28-byte growable arrays:
// the entry is found by key and, unless it is the given array itself, emptied and the given array moved in (the
// shape of src/game/CActor/UnkReflMapMoveSet_b44.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkTileArray_A5_b71 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    void* allocator;

    __attribute__((always_inline)) void MoveFrom(UnkTileArray_A5_b71* o) {
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
static_assert(sizeof(UnkTileArray_A5_b71) == 0x28, "layout");

struct UnkTileFlatMap_A5_b71 {
    UnkTileArray_A5_b71* values;
    u64* keys;
    u32 count;

    UnkTileArray_A5_b71* End() { return &values[count]; }
    UnkTileArray_A5_b71* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_710076037c(UnkTileFlatMap_A5_b71* self, const u64* key, UnkTileArray_A5_b71* value) {
    UnkTileArray_A5_b71* dst = self->Find(key);
    if (dst != self->End())
        dst->MoveFrom(value);
}

// The copy slot (+0x140) of the same map, over the engine array of 16-byte elements: the entry's elements are
// dropped, its array grown to the given array's capacity when smaller, and the elements copied in.
struct UnkTileElem_A5_b71 {
    u64 a;
    u64 b;
};
using UnkTileVector_A5_b71 = base::UnkVector<UnkTileElem_A5_b71>;

struct UnkTileVecFlatMap_A5_b71 {
    UnkTileVector_A5_b71* values;
    u64* keys;
    u32 count;

    UnkTileVector_A5_b71* End() { return &values[count]; }
    UnkTileVector_A5_b71* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

static inline __attribute__((always_inline)) void CopyFrom_A5_b71(UnkTileVector_A5_b71* self,
                                                                  const UnkTileVector_A5_b71* o) {
    if (self == o)
        return;
    self->Clear();
    self->Reserve(o->capacity);
    __builtin_memcpy(self->data, o->data, (u64)o->count * sizeof(UnkTileElem_A5_b71));
    self->count = o->count;
}

void fn_7100760260(UnkTileVecFlatMap_A5_b71* self, const u64* key, const UnkTileVector_A5_b71* value) {
    UnkTileVector_A5_b71* dst = self->Find(key);
    if (dst != self->End())
        CopyFrom_A5_b71(dst, value);
}
