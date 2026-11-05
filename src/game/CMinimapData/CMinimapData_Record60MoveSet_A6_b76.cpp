// CMinimapData's unit: reflected flat maps' "set by key" move slots (CCollectionType +0x148;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and 0x60-byte records (two engine arrays and
// four words, as UnkRecord60 in src/game_aca57c/fn_7100e5493c.cpp) moved in member by member. The b72 shape
// (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkRecord60Move_A6_b76 {
    base::UnkVector<u64> field_0x0;
    base::UnkVector<u16> field_0x28;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    __attribute__((always_inline)) UnkRecord60Move_A6_b76& operator=(UnkRecord60Move_A6_b76&& o) {
        field_0x0 = static_cast<base::UnkVector<u64>&&>(o.field_0x0);
        field_0x28 = static_cast<base::UnkVector<u16>&&>(o.field_0x28);
        field_0x50 = o.field_0x50;
        field_0x54 = o.field_0x54;
        field_0x58 = o.field_0x58;
        field_0x5c = o.field_0x5c;
        return *this;
    }
};
static_assert(sizeof(UnkRecord60Move_A6_b76) == 0x60, "layout");

struct UnkRecord60FlatMap_A6_b76 {
    UnkRecord60Move_A6_b76* values;
    u64* keys;
    u32 count;

    UnkRecord60Move_A6_b76* End() { return &values[count]; }
    UnkRecord60Move_A6_b76* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

static inline __attribute__((always_inline)) void MoveSet_A6_b76(UnkRecord60FlatMap_A6_b76* self, const u64* key,
                                                                  UnkRecord60Move_A6_b76* value) {
    UnkRecord60Move_A6_b76* dst = self->Find(key);
    if (dst != self->End())
        *dst = static_cast<UnkRecord60Move_A6_b76&&>(*value);
}

void fn_7100e54644(UnkRecord60FlatMap_A6_b76* self, const u64* key, UnkRecord60Move_A6_b76* value) {
    MoveSet_A6_b76(self, key, value);
}

void fn_7100e56378(UnkRecord60FlatMap_A6_b76* self, const u64* key, UnkRecord60Move_A6_b76* value) {
    MoveSet_A6_b76(self, key, value);
}
