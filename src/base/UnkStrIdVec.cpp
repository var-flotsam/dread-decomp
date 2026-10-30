// Reserve of the string-id vector (base::UnkStrIdVec, base/UnkCloneRecords.h): a larger array
// is allocated through the vector's allocator, each id is moved over bitwise (the source is
// left holding the empty id, then released), and the old array is freed.
#include "base/UnkCloneRecords.h"

void fn_710000542c(base::UnkStrIdVec* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* ids = static_cast<base::global::CStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(base::global::CStrId), 8, self->flags));
        if (base::global::CStrId* old = self->data) {
            // A 64-bit index for the new array beside the 32-bit counter: the end test compares
            // 32 bits.
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                __builtin_memcpy(&ids[j], old, sizeof(base::global::CStrId));
                base::global::UnkStringPool* pool = g_unk_7101d47108;
                old->data = &pool->empty;
                if ((pool = g_unk_7101d47108)) {
                    if (old->data != &pool->empty) {
                        --old->data->ref_count;
                        if (old->data->ref_count == 0) {
                            fn_71000803a4(pool, old->data);
                            pool = g_unk_7101d47108;
                        }
                    }
                    old->data = &pool->empty;
                }
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = ids;
        self->capacity = capacity;
    }
}
