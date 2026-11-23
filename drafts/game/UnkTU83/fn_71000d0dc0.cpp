// Unit 83 (base::reflection): erases the file path id at an iterator from a vector of them, the later
// ones moved down one, and returns the iterator to the one that followed. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkFilePathStrIdBase;

// Releases the id and leaves the empty one.
static inline void Reset(CFilePathStrId* id) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                StringPool_Release(pool, id->data);
                pool = g_unk_7101d47200;
            }
        }
        id->data = &pool->empty;
    }
}

// Takes the id and leaves the empty one behind.
static inline void MoveConstruct(CFilePathStrId* dst, CFilePathStrId* src) {
    new (dst) UnkFilePathStrIdBase(*src);
    src->data = &g_unk_7101d47200->empty;
}

CFilePathStrId* fn_71000d0dc0(base::global::CRntVector<CFilePathStrId>* self, CFilePathStrId* it) {
    u32 index = ((u64)it - (u64)self->data) / sizeof(CFilePathStrId);
    if (index < self->count) {
        Reset(&self->data[index]);
        if (self->count - 1 > index) {
            u64 i = index;
            do {
                MoveConstruct(&self->data[i], &self->data[i + 1]);
                self->data[i + 1].~CFilePathStrId();
                ++i;
            } while (i < self->count - 1);
        }
        self->count = self->count - 1;
    }
    return self->data + index;
}
