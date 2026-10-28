// Grows the capacity of a pair of parallel arrays (string id + 8-byte value entries, and string
// ids) to at least `capacity`: allocates both, copies the entries and moves the ids over,
// destroys and frees the old arrays.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;


// A string id whose destructor decrements and tests in two steps.
struct UnkId_71004f1558 {
    UnkStringData* data;

    UnkId_71004f1558(const UnkId_71004f1558& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    // Moves bitwise: the original reloads the array count after the copy, which a typed
    // pointer store wouldn't force.
    UnkId_71004f1558(UnkId_71004f1558&& other) {
        __builtin_memcpy(this, &other, sizeof(*this));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkId_71004f1558() {
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

struct UnkEntry_71004f1558 {
    UnkId_71004f1558 key;
    u64 value;
};

struct UnkStruct_71004f1558 {
    UnkEntry_71004f1558* entries;
    UnkId_71004f1558* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_71004f1558(UnkStruct_71004f1558* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkEntry_71004f1558* entries = static_cast<UnkEntry_71004f1558*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkEntry_71004f1558), 8, self->flags));
    UnkId_71004f1558* ids = static_cast<UnkId_71004f1558*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkId_71004f1558), 8, self->flags));
    if (UnkEntry_71004f1558* src = self->entries) {
        UnkEntry_71004f1558* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkEntry_71004f1558(*src);
            src->~UnkEntry_71004f1558();
        }
        self->allocator->vf_0x48(self->entries);
    }
    if (UnkId_71004f1558* src = self->ids) {
        UnkId_71004f1558* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkId_71004f1558(static_cast<UnkId_71004f1558&&>(*src));
            src->~UnkId_71004f1558();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->entries = entries;
    self->ids = ids;
    self->capacity = capacity;
}
