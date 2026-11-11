// base::reflection (unit 2610): resets a record to its default state, releasing every string id
// it holds (nine ids, two id arrays, a name and a file path) and clearing its trailing values.
// Names and layouts are placeholders.

#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

namespace {

struct UnkIdArray {
    UnkStringData** data;
    u32 count;
    u32 pad_0xc;
};

struct UnkReflRecord {
    u8 pad_0x0[0x20];
    UnkStringData* path;  // an id of the file-path pool
    CStrId name;
    CStrId ids[9];
    UnkIdArray idArray;
    u8 pad_0x88[0x18];
    UnkIdArray pathArray;  // ids of the file-path pool
    u8 pad_0xb0[0x18];
    f32 field_0xc8;
    u16 field_0xcc;
};
static_assert(__builtin_offsetof(UnkReflRecord, ids) == 0x30);
static_assert(__builtin_offsetof(UnkReflRecord, idArray) == 0x78);
static_assert(__builtin_offsetof(UnkReflRecord, pathArray) == 0xa0);
static_assert(__builtin_offsetof(UnkReflRecord, field_0xc8) == 0xc8);

// Releases an id of the given pool in two steps and leaves the pool's empty id.
inline void ResetId(UnkStringData*& data, UnkStringPool*& poolRef) {
    if (UnkStringPool* pool = poolRef) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = poolRef;
            }
        }
        data = &pool->empty;
    }
}

inline void ClearIds(UnkIdArray& array, UnkStringPool*& poolRef) {
    for (u64 i = 0; i < array.count; ++i)
        ResetId(array.data[i], poolRef);
    array.count = 0;
}

}  // namespace

void fn_7100fa64bc(void* self) {
    UnkReflRecord* record = static_cast<UnkReflRecord*>(self);

    record->ids[0].Reset();
    record->ids[1].Reset();
    record->ids[2].Reset();
    record->ids[3].Reset();
    record->ids[4].Reset();
    record->ids[5].Reset();
    record->ids[6].Reset();
    record->ids[7].Reset();
    record->ids[8].Reset();
    ClearIds(record->idArray, g_unk_7101d47108);
    record->name = g_unk_7101d470e0;
    ClearIds(record->pathArray, g_unk_7101d47200);
    ResetId(record->path, g_unk_7101d47200);
    record->field_0xc8 = -1.0f;
    record->field_0xcc = 0;
}
