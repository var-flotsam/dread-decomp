// Shrink-to-fit of a container of two parallel arrays (pointer-sized values and string ids)
// sharing a count and capacity: both are reallocated to exactly `count`, the values copied,
// the ids copied and destroyed one by one, the old arrays freed.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkAllocator;
using base::global::CStrId;

struct UnkStrIdTable {
    void** values;
    CStrId* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;
};

// CStrId's destructor, decrementing and testing in two steps (the original reloads the id
// after the decrement).
static inline void Release(CStrId* id) {
    using namespace base::global;
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = g_unk_7101d47108;
            }
        }
        id->data = &pool->empty;
    }
}

void fn_7100d86d30(UnkStrIdTable* self) {
    if (self->count == self->capacity)
        return;
    void** values = static_cast<void**>(self->allocator->vf_0x40((u64)self->count * sizeof(void*), 8, self->flags));
    CStrId* ids = static_cast<CStrId*>(self->allocator->vf_0x40((u64)self->count * sizeof(CStrId), 8, self->flags));
    if (self->values) {
        __builtin_memcpy(values, self->values, (u64)self->count * sizeof(void*));
        self->allocator->vf_0x48(self->values);
    }
    if (CStrId* old = self->ids) {
        CStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) CStrId(*old);
            Release(old);
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->values = values;
    self->ids = ids;
    self->capacity = self->count;
}
