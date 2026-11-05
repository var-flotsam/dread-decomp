// CBossRushSurvivalConfiguration's unit: a reflected flat map's erase-by-key slot (CCollectionType +0x128;
// src/reflection_types/collection_types_00.cpp) with word keys and word values: the entry found is replaced by
// the last one and the count dropped. Names and layouts are placeholders.
#include "types.h"

struct UnkWordFlatMap_A6_b76 {
    u32* values;
    u32* keys;
    u32 count;

    u32* End() { return &values[count]; }
    u32* Find(const u32* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }

    __attribute__((always_inline)) void EraseAt(u32 index) {
        if (index != count - 1) {
            values[index] = values[count - 1];
            keys[index] = keys[count - 1];
        }
        --count;
    }
};

void fn_7100dd0798(UnkWordFlatMap_A6_b76* self, const u32* key) {
    u32* v = self->Find(key);
    if (v != self->End())
        self->EraseAt((u32)(v - self->values));
}
