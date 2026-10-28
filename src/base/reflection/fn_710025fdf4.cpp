// Grows the capacity of a pair of parallel arrays (0x60-byte polymorphic entries, and string
// ids) to at least `capacity`: allocates both, copies the entries and moves the ids over,
// destroys and frees the old arrays.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;


// A string id whose destructor decrements and tests in two steps.
struct UnkId_710025fdf4 {
    UnkStringData* data;

    UnkId_710025fdf4(const UnkId_710025fdf4& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    // Moves bitwise: the original reloads the array count after the copy, which a typed
    // pointer store wouldn't force.
    UnkId_710025fdf4(UnkId_710025fdf4&& other) {
        __builtin_memcpy(this, &other, sizeof(*this));
        other.data = &g_unk_7101d47108->empty;
    }
    ~UnkId_710025fdf4() {
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

// A polymorphic 0x60-byte entry: copied out of line, destroyed through its vtable.
struct UnkEntry_710025fdf4 {
    virtual ~UnkEntry_710025fdf4();
    UnkEntry_710025fdf4(const UnkEntry_710025fdf4& other);  // fn_7100262f74
    u8 unk_0x8[0x58];
};

struct UnkStruct_710025fdf4 {
    UnkEntry_710025fdf4* entries;
    UnkId_710025fdf4* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

// NON_MATCHING(minor): in the entry loop, the pointer copy and the count compare are scheduled
// the other way round.
void fn_710025fdf4(UnkStruct_710025fdf4* self, u32 capacity) {
    if (self->capacity >= capacity)
        return;
    UnkEntry_710025fdf4* entries = static_cast<UnkEntry_710025fdf4*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkEntry_710025fdf4), 8, self->flags));
    UnkId_710025fdf4* ids = static_cast<UnkId_710025fdf4*>(
        self->allocator->vf_0x40((u64)capacity * sizeof(UnkId_710025fdf4), 8, self->flags));
    if (UnkEntry_710025fdf4* src = self->entries) {
        UnkEntry_710025fdf4* dst = entries;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkEntry_710025fdf4(*src);
            src->~UnkEntry_710025fdf4();
        }
        self->allocator->vf_0x48(self->entries);
    }
    if (UnkId_710025fdf4* src = self->ids) {
        UnkId_710025fdf4* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
            new (dst) UnkId_710025fdf4(static_cast<UnkId_710025fdf4&&>(*src));
            src->~UnkId_710025fdf4();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->entries = entries;
    self->ids = ids;
    self->capacity = capacity;
}
