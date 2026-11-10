// Unit 2563 of tools/tu_map.py (no reliable name; hint CMinimapData): clearing a table of 0x60-byte list pairs keyed by string ids: the keys are
// released, each value's two lists freed, and the count zeroed. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRec60IdArray_b0223;

struct UnkListPair_o0312 {
    base::UnkVector<void*> list0{0xf};
    base::UnkVector<void*> list1{0xf};
    f32 min[2] = {__FLT_MAX__, __FLT_MAX__};
    f32 max[2] = {-__FLT_MAX__, -__FLT_MAX__};
    ~UnkListPair_o0312() {
        list1.Free();
        list0.Free();
    }
};
static_assert(sizeof(UnkListPair_o0312) == 0x60, "layout");

struct UnkReleasedStrId_o0312 {
    base::global::UnkStringData* data;
    __attribute__((always_inline)) ~UnkReleasedStrId_o0312() {
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
};

struct UnkIdTable_o0312 {
    UnkListPair_o0312* values;
    UnkReleasedStrId_o0312* keys;
    u32 count;
};

void fn_7100e4efa0(UnkRec60IdArray_b0223* table) {
    UnkIdTable_o0312* self = reinterpret_cast<UnkIdTable_o0312*>(table);
    if (self->keys && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkReleasedStrId_o0312();
    }
    if (self->values && self->count) {
        u64 offset = 0;
        for (u64 i = 0; i < self->count; offset += sizeof(UnkListPair_o0312), ++i)
            reinterpret_cast<UnkListPair_o0312*>(reinterpret_cast<u8*>(self->values) + offset)->~UnkListPair_o0312();
    }
    self->count = 0;
}
