// Grows a list of file-path ids (base/UnkResPathVec.h): a larger array is allocated through the
// list's allocator, each id is moved over bitwise (the source is left holding the empty id, then
// released), and the old array is freed. The string-id list's copy is src/base/UnkStrIdVec.cpp.
#include "base/UnkResPathVec.h"

using base::global::CFilePathStrId;
using base::global::UnkStringPool;

void fn_7100018450(UnkResPathVec* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* ids = static_cast<CFilePathStrId*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(CFilePathStrId), 8, self->flags));
        if (CFilePathStrId* old = self->data) {
            // A 64-bit index for the new array beside the 32-bit counter: the end test compares
            // 32 bits.
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j, ++old) {
                __builtin_memcpy(&ids[j], old, sizeof(CFilePathStrId));
                UnkStringPool* pool = g_unk_7101d47200;
                old->data = &pool->empty;
                if ((pool = g_unk_7101d47200)) {
                    if (old->data != &pool->empty) {
                        --old->data->ref_count;
                        if (old->data->ref_count == 0) {
                            fn_71000803a4(pool, old->data);
                            pool = g_unk_7101d47200;
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
