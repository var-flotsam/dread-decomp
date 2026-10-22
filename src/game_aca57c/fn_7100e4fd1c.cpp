// Copy constructor and destructor of a 0x60-byte record (the values of the id map assigned by
// fn_7100e59f50): plain words, two string ids, and an array of string ids (base::UnkVector's
// layout, with an out-of-line Reserve, fn_710000542c) copied by appending each id.
#include <new>

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
};
void fn_710000542c(UnkReleasedStrIdArray* self, u32 capacity);

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
    fn_710000542c(ids, other->ids.capacity);
    for (const UnkReleasedStrId *it = other->ids.data, *end = it + other->ids.count; it != end; ++it) {
        u32 n = ids->count;
        u32 new_capacity = ids->capacity;
        if (new_capacity <= n && ids->step != 0)
            new_capacity = (n / ids->step + 1) * ids->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_710000542c(ids, new_capacity);
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
