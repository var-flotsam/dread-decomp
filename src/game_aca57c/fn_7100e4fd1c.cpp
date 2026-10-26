// Copy constructor and destructor of a 0x60-byte record (the values of the id map assigned by
// fn_7100e59f50): plain words, two string ids, and an array of string ids (base::UnkVector's
// layout, with an out-of-line Reserve, fn_710000542c) copied by appending each id.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
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

struct UnkReleasedStrIdArray {
    UnkReleasedStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkReleasedStrIdArray() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~UnkReleasedStrId();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }

    // The out-of-line Reserve takes the header's vector of string ids, which has this layout.
    void Reserve(u32 n) { fn_710000542c(reinterpret_cast<base::UnkStrIdVec*>(this), n); }
};

struct UnkIdsRecord60 {
    u32 field_0x0[10];
    UnkReleasedStrId id_0x28;
    UnkReleasedStrId id_0x30;
    UnkReleasedStrIdArray ids;

    __attribute__((always_inline)) ~UnkIdsRecord60() {}
};
static_assert(sizeof(UnkIdsRecord60) == 0x60, "UnkIdsRecord60 layout");

void fn_7100e4fd1c(UnkIdsRecord60* self, const UnkIdsRecord60* other) {
    for (u32 i = 0; i < 10; ++i)
        self->field_0x0[i] = other->field_0x0[i];
    UnkStringData* data = other->id_0x28.data;
    self->id_0x28.data = data;
    UnkStringData* empty = &g_unk_7101d47108->empty;
    if (empty != data)
        ++data->ref_count;
    data = other->id_0x30.data;
    self->id_0x30.data = data;
    if (empty != data)
        ++data->ref_count;
    UnkReleasedStrIdArray* ids = &self->ids;
    ids->data = nullptr;
    ids->count = 0;
    ids->capacity = 0;
    ids->step = other->ids.step;
    ids->field_0x14 = other->ids.field_0x14;
    ids->flags = other->ids.flags;
    ids->allocator = other->ids.allocator;
    ids->Reserve(other->ids.capacity);
    for (const UnkReleasedStrId *it = other->ids.data, *end = it + other->ids.count; it != end; ++it) {
        u32 n = ids->count;
        u32 new_capacity = ids->capacity;
        if (new_capacity <= n && ids->step != 0)
            new_capacity = (n / ids->step + 1) * ids->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        ids->Reserve(new_capacity);
        // A bitwise copy of the id (the original reloads the count after it), then the reference.
        UnkReleasedStrId* slot = &ids->data[ids->count];
        UnkStringData* d = it->data;
        __builtin_memcpy(slot, &d, sizeof(d));
        if (&g_unk_7101d47108->empty != d)
            ++d->ref_count;
        ++ids->count;
    }
}

void fn_7100e4a1d0(UnkIdsRecord60* self) { self->~UnkIdsRecord60(); }

// Move-assignment: copies the words, releases and takes over each id (leaving the pool's empty
// string), and unless assigning to itself, releases the ids in the array and takes over the
// other's array (the old storage is not freed).
__attribute__((always_inline)) static inline void MoveAssign(UnkReleasedStrIdArray* self,
                                                            UnkReleasedStrIdArray* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkReleasedStrId();
        self->count = 0;
        self->allocator = other->allocator;
        self->flags = other->flags;
        self->step = other->step;
        self->field_0x14 = other->field_0x14;
        self->count = other->count;
        self->capacity = other->capacity;
        self->data = other->data;
        other->data = nullptr;
        other->count = 0;
        other->capacity = 0;
    }
}

__attribute__((always_inline)) static inline void MoveId(UnkReleasedStrId* self, UnkReleasedStrId* other) {
    self->~UnkReleasedStrId();
    self->data = other->data;
    other->data = &g_unk_7101d47108->empty;
}

UnkIdsRecord60* fn_7100e4fff8(UnkIdsRecord60* self, UnkIdsRecord60* other) {
    for (u32 i = 0; i < 10; ++i)
        self->field_0x0[i] = other->field_0x0[i];
    MoveId(&self->id_0x28, &other->id_0x28);
    MoveId(&self->id_0x30, &other->id_0x30);
    MoveAssign(&self->ids, &other->ids);
    return self;
}
