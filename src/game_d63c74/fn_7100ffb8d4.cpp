// Free of an array (base::UnkVector's layout) of file path ids paired with two words, as
// fn_71011df574 (src/game/GUI/CHUD/fn_71011df574.cpp) with the ids in CFilePathStrId's pool:
// fn_7100ffb8d4. Each id is released (and left as the empty id) before the array is freed.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkEmptiedFilePathStrId {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkEmptiedFilePathStrId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkFilePathIdWordPair {
    UnkEmptiedFilePathStrId id;
    u64 field_0x8;
    u64 field_0x10;
};

void fn_7100ffb8d4(base::UnkVector<UnkFilePathIdWordPair>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkFilePathIdWordPair();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}
