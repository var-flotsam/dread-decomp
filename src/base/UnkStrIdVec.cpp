// Reserve of the string-id vector (base::UnkStrIdVec, base/UnkCloneRecords.h): a larger array
// is allocated through the vector's allocator, each id is moved over bitwise (the source is
// left holding the empty id, then released), and the old array is freed.
#include "base/UnkCloneRecords.h"

// NON_MATCHING(minor): same behaviour; the loop's end test compares the widened index with the
// count as 64-bit (cmp x, x) where the original compares 32 bits (98.5%, one instruction).
void fn_710000542c(base::UnkStrIdVec* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* ids = static_cast<base::global::CStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(base::global::CStrId), 8, self->flags));
        base::global::CStrId* old = self->data;
        if (old) {
            for (u32 i = 0; i < self->count; ++i) {
                __builtin_memcpy(&ids[i], &old[i], sizeof(base::global::CStrId));
                base::global::UnkStringPool* pool = g_unk_7101d47108;
                old[i].data = &pool->empty;
                if ((pool = g_unk_7101d47108)) {
                    if (old[i].data != &pool->empty) {
                        --old[i].data->ref_count;
                        if (old[i].data->ref_count == 0) {
                            fn_71000803a4(pool, old[i].data);
                            pool = g_unk_7101d47108;
                        }
                    }
                    old[i].data = &pool->empty;
                }
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = ids;
        self->capacity = capacity;
    }
}
