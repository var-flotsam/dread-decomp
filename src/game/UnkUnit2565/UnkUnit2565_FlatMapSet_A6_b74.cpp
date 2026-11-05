// Unit 2565 (tools/tu_map.py): a reflected flat map's "set by key" slots (CCollectionType +0x140 copy,
// +0x148 move; src/reflection_types/collection_types_00.cpp) with u32 keys and 0x248-byte string values
// assigned out of line by the run-time string's assignment (fn_710007eaa0); both slots copy. The b72 shape
// (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

struct UnkLongString_A6_b74 {
    u8 data[0x248];

    void Assign(const UnkLongString_A6_b74* o) {
        fn_710007eaa0(reinterpret_cast<base::global::CRntString*>(this),
                      reinterpret_cast<const base::global::CRntString*>(o));
    }
};

struct UnkLongStringFlatMap_A6_b74 {
    UnkLongString_A6_b74* values;
    u32* keys;
    u32 count;

    UnkLongString_A6_b74* End() { return &values[count]; }
    UnkLongString_A6_b74* Find(const u32* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

static inline __attribute__((always_inline)) void Set_A6_b74(UnkLongStringFlatMap_A6_b74* self, const u32* key,
                                                             const UnkLongString_A6_b74* value) {
    UnkLongString_A6_b74* dst = self->Find(key);
    if (dst != self->End())
        dst->Assign(value);
}

void fn_7100e7b880(UnkLongStringFlatMap_A6_b74* self, const u32* key, const UnkLongString_A6_b74* value) {
    Set_A6_b74(self, key, value);
}

void fn_7100e7b900(UnkLongStringFlatMap_A6_b74* self, const u32* key, UnkLongString_A6_b74* value) {
    Set_A6_b74(self, key, value);
}
