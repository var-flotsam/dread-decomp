// Reserve and shrink-to-fit of a container of two parallel arrays (0xa8-byte records and string
// ids) sharing a count and capacity: both are reallocated, the records moved over and the old
// ones destroyed (their arrays freed), the ids moved (reserve) or copied (shrink) and the old ones
// released, and the old arrays freed.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// A string id: moving leaves the pool's empty string behind; the destructor decrements and tests
// in two steps (the original reloads the id after the decrement).
struct UnkReleasedStrId {
    UnkStringData* data;

    UnkReleasedStrId(const UnkReleasedStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkReleasedStrId(UnkReleasedStrId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkReleasedStrId() {
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

// base::UnkVector's layout for plain elements: moving takes over the storage, destroying frees it.
struct UnkArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkArray(UnkArray&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkBounds {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;

    UnkBounds() : min_x(3.4028235e38f), min_y(3.4028235e38f), max_x(-3.4028235e38f), max_y(-3.4028235e38f) {}
};

struct UnkRecordA8 {
    UnkArray field_0x0;
    u32 field_0x28;
    bool field_0x2c;
    bool field_0x2d;
    UnkBounds field_0x30;
    u32 field_0x40;
    UnkArray field_0x48;
    UnkArray field_0x70;
    u32 field_0x98;
    u32 field_0x9c;
    u32 field_0xa0;
    u32 field_0xa4;

    // The bounds are reset, not moved.
    __attribute__((always_inline)) UnkRecordA8(UnkRecordA8&& other)
        : field_0x0(static_cast<UnkArray&&>(other.field_0x0)), field_0x28(other.field_0x28),
          field_0x2c(other.field_0x2c), field_0x2d(true), field_0x40(other.field_0x40),
          field_0x48(static_cast<UnkArray&&>(other.field_0x48)), field_0x70(static_cast<UnkArray&&>(other.field_0x70)),
          field_0x98(other.field_0x98), field_0x9c(other.field_0x9c), field_0xa0(other.field_0xa0),
          field_0xa4(other.field_0xa4) {}
};
static_assert(sizeof(UnkRecordA8) == 0xa8, "UnkRecordA8 layout");

struct UnkRecordIdArray {
    UnkRecordA8* records;
    UnkReleasedStrId* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

// NON_MATCHING(minor): the original keeps a 32-bit record counter (ours is widened to 64 bits),
// and in the ids loop x8 and x9 are swapped.
void fn_7100e5f348(UnkRecordIdArray* self, u32 n) {
    if (self->capacity >= n)
        return;
    auto* records = static_cast<UnkRecordA8*>(self->allocator->vf_0x40((u64)n * sizeof(UnkRecordA8), 8, self->flags));
    auto* ids = static_cast<UnkReleasedStrId*>(self->allocator->vf_0x40((u64)n * sizeof(UnkReleasedStrId), 8, self->flags));
    if (UnkRecordA8* old = self->records) {
        for (u32 i = 0; i < self->count; ++i) {
            new (&records[i]) UnkRecordA8(static_cast<UnkRecordA8&&>(old[i]));
            old[i].~UnkRecordA8();
        }
        self->allocator->vf_0x48(self->records);
    }
    if (UnkReleasedStrId* old = self->ids) {
        UnkReleasedStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkReleasedStrId(static_cast<UnkReleasedStrId&&>(*old));
            old->~UnkReleasedStrId();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = n;
}

// NON_MATCHING(minor): as fn_7100e5f348 for the record counter; in the ids loop the original
// steps a pointer to the old id alongside the index, ours addresses it by the index.
void fn_7100e5f748(UnkRecordIdArray* self) {
    if (self->count == self->capacity)
        return;
    auto* records = static_cast<UnkRecordA8*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkRecordA8), 8, self->flags));
    auto* ids = static_cast<UnkReleasedStrId*>(
        self->allocator->vf_0x40((u64)self->count * sizeof(UnkReleasedStrId), 8, self->flags));
    if (UnkRecordA8* old = self->records) {
        for (u32 i = 0; i < self->count; ++i) {
            new (&records[i]) UnkRecordA8(static_cast<UnkRecordA8&&>(old[i]));
            old[i].~UnkRecordA8();
        }
        self->allocator->vf_0x48(self->records);
    }
    if (UnkReleasedStrId* old = self->ids) {
        UnkReleasedStrId* dst = ids;
        for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
            new (dst) UnkReleasedStrId(*old);
            old->~UnkReleasedStrId();
        }
        self->allocator->vf_0x48(self->ids);
    }
    self->records = records;
    self->ids = ids;
    self->capacity = self->count;
}
