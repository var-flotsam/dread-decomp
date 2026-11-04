// CMinimapData's unit: a reflected flat map's "set by key" move slot (CCollectionType +0x148;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and 0x80-byte records moved in by the
// record's assignment (fn_7100e51a60, src/game_aca57c/fn_7100e509a4.cpp). The b72 shape
// (src/game/CNavMeshItemComponent/UnkReflFlatMapSet_A4_b72.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkRecord80;
UnkRecord80* fn_7100e51a60(UnkRecord80* self, UnkRecord80* other);

struct UnkRecord80Slot_A6_b74 {
    u8 data[0x80];
};

struct UnkRecord80FlatMap_A6_b74 {
    UnkRecord80Slot_A6_b74* values;
    u64* keys;
    u32 count;

    UnkRecord80Slot_A6_b74* End() { return &values[count]; }
    UnkRecord80Slot_A6_b74* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100e60af4(UnkRecord80FlatMap_A6_b74* self, const u64* key, UnkRecord80Slot_A6_b74* value) {
    UnkRecord80Slot_A6_b74* dst = self->Find(key);
    if (dst != self->End()) {
        fn_7100e51a60(reinterpret_cast<UnkRecord80*>(dst), reinterpret_cast<UnkRecord80*>(value));
        // The original keeps the assignment a plain call.
        asm volatile("");
    }
}
