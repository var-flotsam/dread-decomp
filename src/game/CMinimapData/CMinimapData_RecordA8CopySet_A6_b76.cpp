// CMinimapData's unit: a reflected flat map's "set by key" copy slot (CCollectionType +0x140;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and 0xa8-byte records: the {word, byte} value
// is copied with its set flag raised, the leading array assigned out of line (fn_710011dac0), a word copied and
// the trailing arrays assigned out of line (fn_7100e4f444). The b72 shape
// (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "game/UnkTU108.h"
#include "game/UnkTU2563.h"
#include "types.h"

struct UnkPartsRecord;
struct UnkArraysRecord;

struct alignas(8) UnkRecordA8Copy_A6_b76 {
    u8 field_0x0[0x28];
    u32 field_0x28;
    u8 field_0x2c;
    bool field_0x2d;
    u8 unk_0x2e[0x12];
    u32 field_0x40;
    u32 unk_0x44;
    u8 field_0x48[0x60];

    __attribute__((always_inline)) UnkRecordA8Copy_A6_b76& operator=(const UnkRecordA8Copy_A6_b76& o) {
        field_0x28 = o.field_0x28;
        field_0x2c = o.field_0x2c;
        field_0x2d = true;
        fn_710011dac0(reinterpret_cast<UnkPartsRecord*>(field_0x0),
                      reinterpret_cast<const UnkPartsRecord*>(o.field_0x0));
        field_0x40 = o.field_0x40;
        fn_7100e4f444(reinterpret_cast<UnkArraysRecord*>(field_0x48),
                      reinterpret_cast<const UnkArraysRecord*>(o.field_0x48));
        return *this;
    }
};
static_assert(sizeof(UnkRecordA8Copy_A6_b76) == 0xa8, "layout");

struct UnkRecordA8CopyFlatMap_A6_b76 {
    UnkRecordA8Copy_A6_b76* values;
    u64* keys;
    u32 count;

    UnkRecordA8Copy_A6_b76* End() { return &values[count]; }
    UnkRecordA8Copy_A6_b76* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100e5ee6c(UnkRecordA8CopyFlatMap_A6_b76* self, const u64* key, const UnkRecordA8Copy_A6_b76* value) {
    UnkRecordA8Copy_A6_b76* dst = self->Find(key);
    if (dst != self->End()) {
        *dst = *value;
        // The original keeps the last assignment a plain call.
        asm volatile("");
    }
}
