// CMinimapDef's unit: a reflected flat map's "set by key" move slot (CCollectionType +0x148, copy +0x140;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and 0x18-byte records (a string id and four
// words). The id is taken over with CStrId::ReleaseAndTake, the words copied. The b72 shape
// (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdRecord18_A6_b76 {
    base::global::CStrId id;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;

    __attribute__((always_inline)) void MoveFrom(UnkIdRecord18_A6_b76* o) {
        id.ReleaseAndTake(o->id);
        field_0x8 = o->field_0x8;
        field_0xc = o->field_0xc;
        field_0x10 = o->field_0x10;
        field_0x14 = o->field_0x14;
    }
    __attribute__((always_inline)) void CopyFrom(const UnkIdRecord18_A6_b76* o) {
        id = o->id;
        field_0x8 = o->field_0x8;
        field_0xc = o->field_0xc;
        field_0x10 = o->field_0x10;
        field_0x14 = o->field_0x14;
    }
};
static_assert(sizeof(UnkIdRecord18_A6_b76) == 0x18, "layout");

struct UnkIdRecord18FlatMap_A6_b76 {
    UnkIdRecord18_A6_b76* values;
    u64* keys;
    u32 count;

    UnkIdRecord18_A6_b76* End() { return &values[count]; }
    UnkIdRecord18_A6_b76* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100e715a4(UnkIdRecord18FlatMap_A6_b76* self, const u64* key, UnkIdRecord18_A6_b76* value) {
    UnkIdRecord18_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        dst->MoveFrom(value);
}

// The copy slot (+0x140) of the same map: the id is copy-assigned (CStrId's operator=), the words copied.
void fn_7100e71488(UnkIdRecord18FlatMap_A6_b76* self, const u64* key, const UnkIdRecord18_A6_b76* value) {
    UnkIdRecord18_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        dst->CopyFrom(value);
}
