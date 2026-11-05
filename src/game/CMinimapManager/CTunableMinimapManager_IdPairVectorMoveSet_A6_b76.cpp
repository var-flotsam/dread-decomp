// CTunableMinimapManager's unit: a reflected flat map's "set by key" move slot (CCollectionType +0x148;
// src/reflection_types/collection_types_00.cpp) with 8-byte keys and engine arrays (base::UnkVector) of 0x10-byte
// {string id, word} records as values, moved in by the array's move assignment. The ids are released in two
// steps (as CStrId::Reset). As src/game/CMinimapData/CMinimapData_IdVectorMoveSet_A6_b76.cpp. Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdElem_A6_b76m : base::global::UnkStrIdBase {
    ~UnkIdElem_A6_b76m() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
    u64 field_0x8;
};

using UnkIdVector_A6_b76m = base::UnkVector<UnkIdElem_A6_b76m>;

struct UnkIdVectorFlatMap_A6_b76m {
    UnkIdVector_A6_b76m* values;
    u64* keys;
    u32 count;

    UnkIdVector_A6_b76m* End() { return &values[count]; }
    UnkIdVector_A6_b76m* Find(const u64* key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == *key)
                return values + i;
        return values + count;
    }
};

void fn_7100eb1078(UnkIdVectorFlatMap_A6_b76m* self, const u64* key, UnkIdVector_A6_b76m* value) {
    UnkIdVector_A6_b76m* dst = self->Find(key);
    if (dst != self->End())
        *dst = static_cast<UnkIdVector_A6_b76m&&>(*value);
}
