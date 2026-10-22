// Two tables' Clear: the string ids in the key array (at 8) are released, then the ids in the
// records of the value array (at 0; an id at 0x18 of 0x28 bytes, or at 0 of 0x18 bytes), and the
// count (at 0x10) is zeroed. The arrays themselves are kept.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose destructor decrements and tests in two steps (the original reloads the id
// after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkReleasedStrId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
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

struct UnkIdRecord28 {
    u64 field_0x0[3];
    UnkReleasedStrId id;
    u64 field_0x20;
};

struct UnkIdRecord18 {
    UnkReleasedStrId id;
    u64 field_0x8[2];
};

template <class Value>
struct UnkIdRecordTable {
    Value* values;
    UnkReleasedStrId* keys;
    u32 count;
};

template <class Value>
__attribute__((always_inline)) static inline void Clear(UnkIdRecordTable<Value>* self) {
    if (self->keys && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->keys[i].~UnkReleasedStrId();
    }
    if (self->values && self->count) {
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].~Value();
    }
    self->count = 0;
}

void fn_7100e4ee60(UnkIdRecordTable<UnkIdRecord28>* self) { Clear(self); }
void fn_7100e71cc4(UnkIdRecordTable<UnkIdRecord18>* self) { Clear(self); }
