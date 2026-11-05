// CMinimapData's unit: a reflected flat map's "set by key" move slot (CCollectionType +0x148, copy +0x140;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and 0x28-byte records (six words, a string id
// and a half-word). The words are copied, the id taken over with CStrId::ReleaseAndTake, then the half-word.
// The b72 shape (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdRecord28_A6_b76 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    base::global::CStrId id;
    u16 field_0x20;

    __attribute__((always_inline)) void MoveFrom(UnkIdRecord28_A6_b76* o) {
        field_0x0 = o->field_0x0;
        field_0x4 = o->field_0x4;
        field_0x8 = o->field_0x8;
        field_0xc = o->field_0xc;
        field_0x10 = o->field_0x10;
        field_0x14 = o->field_0x14;
        id.ReleaseAndTake(o->id);
        field_0x20 = o->field_0x20;
    }
    __attribute__((always_inline)) void CopyFrom(const UnkIdRecord28_A6_b76* o) {
        field_0x0 = o->field_0x0;
        field_0x4 = o->field_0x4;
        field_0x8 = o->field_0x8;
        field_0xc = o->field_0xc;
        field_0x10 = o->field_0x10;
        field_0x14 = o->field_0x14;
        id = o->id;
        field_0x20 = o->field_0x20;
    }
};
static_assert(sizeof(UnkIdRecord28_A6_b76) == 0x28, "layout");

struct UnkIdRecord28FlatMap_A6_b76 {
    UnkIdRecord28_A6_b76* values;
    u64* keys;
    u32 count;

    UnkIdRecord28_A6_b76* End() { return &values[count]; }
    UnkIdRecord28_A6_b76* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100e5b670(UnkIdRecord28FlatMap_A6_b76* self, const u64* key, UnkIdRecord28_A6_b76* value) {
    UnkIdRecord28_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        dst->MoveFrom(value);
}

// The copy slot (+0x140): the same with the id copy-assigned (CStrId's operator=).
void fn_7100e5b530(UnkIdRecord28FlatMap_A6_b76* self, const u64* key, const UnkIdRecord28_A6_b76* value) {
    UnkIdRecord28_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        dst->CopyFrom(value);
}
