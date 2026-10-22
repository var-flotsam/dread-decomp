// Copy constructor of a container of two parallel u32 arrays sharing a count and capacity:
// takes the source's settings, reserves its count and copies both arrays.
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;

struct UnkU32PairArray {
    u32* values;
    u32* ids;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    UnkAllocator* allocator;

    UnkU32PairArray(const UnkU32PairArray& other);

    void Reserve(u32 n) {
        if (capacity >= n)
            return;
        u32* new_values = static_cast<u32*>(allocator->vf_0x40((u64)n * sizeof(u32), 8, flags));
        u32* new_ids = static_cast<u32*>(allocator->vf_0x40((u64)n * sizeof(u32), 8, flags));
        if (values) {
            __builtin_memcpy(new_values, values, (u64)count * sizeof(u32));
            allocator->vf_0x48(values);
        }
        if (ids) {
            __builtin_memcpy(new_ids, ids, (u64)count * sizeof(u32));
            allocator->vf_0x48(ids);
        }
        values = new_values;
        ids = new_ids;
        capacity = n;
    }
};

void fn_7100dd0030(UnkU32PairArray* self, const UnkU32PairArray& other) {
    self->values = nullptr;
    self->ids = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->field_0x18 = other.field_0x18;
    self->flags = other.flags;
    self->allocator = other.allocator;
    self->Reserve(other.count);
    __builtin_memcpy(self->values, other.values, (u64)other.count * sizeof(u32));
    self->count = other.count;
    __builtin_memcpy(self->ids, other.ids, (u64)other.count * sizeof(u32));
    self->count = other.count;
}
