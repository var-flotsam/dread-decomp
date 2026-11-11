// CMinimapData's unit: a reflected flat map's "set by key" copy slot (CCollectionType +0x140;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and 0x80-byte records: four words, a string id
// (CStrId's copy assignment), the 0x60-byte array part assigned out of line (fn_7100e4f444) and a word. The b72
// shape (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU2563.h"
#include "types.h"

struct UnkArraysRecord;

struct UnkIdRecord80_A6_b76 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    base::global::CStrId id;
    u8 field_0x18[0x60];
    u32 field_0x78;

    __attribute__((always_inline)) void CopyFrom(const UnkIdRecord80_A6_b76* o) {
        field_0x0 = o->field_0x0;
        field_0x4 = o->field_0x4;
        field_0x8 = o->field_0x8;
        field_0xc = o->field_0xc;
        id = o->id;
        fn_7100e4f444(reinterpret_cast<UnkArraysRecord*>(field_0x18),
                      reinterpret_cast<const UnkArraysRecord*>(o->field_0x18));
        field_0x78 = o->field_0x78;
    }
};
static_assert(sizeof(UnkIdRecord80_A6_b76) == 0x80, "layout");

struct UnkIdRecord80FlatMap_A6_b76 {
    UnkIdRecord80_A6_b76* values;
    u64* keys;
    u32 count;

    UnkIdRecord80_A6_b76* End() { return &values[count]; }
    UnkIdRecord80_A6_b76* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100e609c8(UnkIdRecord80FlatMap_A6_b76* self, const u64* key, const UnkIdRecord80_A6_b76* value) {
    UnkIdRecord80_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        dst->CopyFrom(value);
}
