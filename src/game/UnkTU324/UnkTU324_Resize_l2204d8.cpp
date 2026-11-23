// Unit 324 of tools/tu_map.py: resizing an in-place array of 0xa0-byte records (a count, then the
// records' storage at +4): new records are value-built (three limits, the shared empty file path, a
// string with 0x20 bytes of storage), dropped ones destroyed. Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A file-path id that starts as a copy of the shared one and is released in two steps.
struct UnkPathId_l2204d8 {
    UnkStringData* data;

    UnkPathId_l2204d8() : data(g_unk_7101d470e8.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkPathId_l2204d8() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkRecord_l2204d8 {
    s32 field_0x0 = -1;
    s32 field_0x4 = 0x7fffffff;
    s32 field_0x8 = 0x7fffffff;
    u8 field_0xc[0x14];
    UnkPathId_l2204d8 path;  // 0x20
    u8 field_0x28[0x10];
    UnkBufferString<0x20, g_unk_7101a1b5a0> name;  // 0x38
};
static_assert(sizeof(UnkRecord_l2204d8) == 0xa0, "UnkRecord_l2204d8 layout");

struct UnkRecordArray_l2204d8 {
    u32 count;
    u8 storage[0xa0];  // the records, unaligned after the count; the capacity is unknown

    UnkRecord_l2204d8* Data() { return reinterpret_cast<UnkRecord_l2204d8*>(storage); }
};

void fn_71002204d8(void* array, u32 n) {
    UnkRecordArray_l2204d8* self = static_cast<UnkRecordArray_l2204d8*>(array);
    if (self->count < n) {
        for (u32 i = self->count; i < n; ++i)
            new (&self->Data()[i]) UnkRecord_l2204d8();
    } else if (self->count > n) {
        for (u32 i = n; i < self->count; ++i)
            self->Data()[i].~UnkRecord_l2204d8();
    }
    self->count = n;
}
