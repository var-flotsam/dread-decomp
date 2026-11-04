// Reflected array "move-set at index" slots (CCollectionType +0x148; src/reflection_types/collection_types_00.cpp):
// the element at the index (grown into by the array's out-of-line accessor) releases its string id, takes the
// given element's id and leaves it the pool's empty string, then copies the plain fields. Elements are a string
// id and two words (CMinimapManager::CTunableMinimapManager's unit, fn_7100eaf68c) or a file-path id, a u64 and
// a word (unit 2640 of tools/tu_map.py, fn_7100ffb790). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

// The string id's move: the old string is released and the id reset to the empty string of the pool as it
// stands after the release, then the other's string is taken.
template <base::global::UnkStringPool** Pool>
struct UnkMovedId_A6_b74 {
    base::global::UnkStringData* data;

    __attribute__((always_inline)) void MoveFrom(UnkMovedId_A6_b74& other) {
        base::global::UnkStringPool* pool = *Pool;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
        data = other.data;
        other.data = &(*Pool)->empty;
    }
};

struct UnkIdWords_A6_b74 {
    UnkMovedId_A6_b74<&g_unk_7101d47108> id;
    u32 field_0x8;
    u32 field_0xc;
};

UnkIdWords_A6_b74* fn_7100eb0058(void* self, u32 index);

void fn_7100eaf68c(void* self, const u32* index, UnkIdWords_A6_b74* value) {
    UnkIdWords_A6_b74* dst = fn_7100eb0058(self, *index);
    dst->id.MoveFrom(value->id);
    dst->field_0x8 = value->field_0x8;
    dst->field_0xc = value->field_0xc;
}

struct UnkPathIdWords_A6_b74 {
    UnkMovedId_A6_b74<&g_unk_7101d47200> id;
    struct Tail {
        u64 field_0x0;
        u32 field_0x8;
    } __attribute__((packed, aligned(4))) field_0x8;
};

UnkPathIdWords_A6_b74* fn_7100ffc138(void* self, u32 index);

void fn_7100ffb790(void* self, const u32* index, UnkPathIdWords_A6_b74* value) {
    UnkPathIdWords_A6_b74* dst = fn_7100ffc138(self, *index);
    dst->id.MoveFrom(value->id);
    dst->field_0x8 = value->field_0x8;
}
