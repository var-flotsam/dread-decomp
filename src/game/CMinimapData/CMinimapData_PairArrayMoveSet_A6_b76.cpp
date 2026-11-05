// CMinimapData's unit: a reflected flat map's "set by key" move slot (CCollectionType +0x148;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and arrays of 0x60-byte array pairs with a
// second pointer array alongside (UnkSecondPtrArray<UnkFreedVectorPair>, include/base/UnkFreedVectorPair.h) as
// values, and the clear slot (+0x110) of such an array. The entry's pairs are destroyed (storage kept), then it
// takes the given array's fields and leaves that one empty. The b72 shape
// (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "base/UnkFreedVectorPair.h"
#include "types.h"

using UnkPairPtrArray_A6_b76 = UnkSecondPtrArray<UnkFreedVectorPair>;

static inline __attribute__((always_inline)) void Clear_A6_b76(UnkPairPtrArray_A6_b76* self) {
    if (self->data)
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkFreedVectorPair();
    self->count = 0;
}

static inline __attribute__((always_inline)) void MoveFrom_A6_b76(UnkPairPtrArray_A6_b76* self,
                                                                   UnkPairPtrArray_A6_b76* o) {
    Clear_A6_b76(self);
    self->allocator = o->allocator;
    self->field_0x8 = o->field_0x8;
    self->data = o->data;
    self->capacity = o->capacity;
    self->count = o->count;
    self->field_0x1c = o->field_0x1c;
    self->field_0x18 = o->field_0x18;
    o->count = 0;
    o->data = nullptr;
    o->field_0x8 = nullptr;
}

struct UnkPairPtrArrayFlatMap_A6_b76 {
    UnkPairPtrArray_A6_b76* values;
    u64* keys;
    u32 count;

    UnkPairPtrArray_A6_b76* End() { return &values[count]; }
    UnkPairPtrArray_A6_b76* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100e580cc(UnkPairPtrArrayFlatMap_A6_b76* self, const u64* key, UnkPairPtrArray_A6_b76* value) {
    UnkPairPtrArray_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        MoveFrom_A6_b76(dst, value);
}

// The clear slot (+0x110) of the same array: destroys the pairs, keeping the storage.
void fn_7100e54294(UnkPairPtrArray_A6_b76* self) {
    Clear_A6_b76(self);
}

// The clear slot (+0x110) of the plain array of the same pairs (UnkPairArray): destroys them, keeping the
// storage.
void fn_7100e5261c(UnkPairArray* self) {
    for (u64 i = 0; i < self->count; ++i)
        self->data[i].~UnkFreedVectorPair();
    self->count = 0;
}
