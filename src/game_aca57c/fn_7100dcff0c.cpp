// Move-assignment of four arrays of two parallel value lists (sharing a count and capacity) and
// a trailing value: each array takes over the other's storage and settings, leaving it empty.
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;

struct UnkPairArray {
    void* values;
    void* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;

    UnkPairArray& operator=(UnkPairArray&& other) {
        count = 0;
        allocator = other.allocator;
        ids = other.ids;
        values = other.values;
        capacity = other.capacity;
        count = other.count;
        flags = other.flags;
        step = other.step;
        other.count = 0;
        other.values = nullptr;
        other.ids = nullptr;
        return *this;
    }
};

struct UnkPairArrays {
    UnkPairArray field_0x0;
    UnkPairArray field_0x28;
    UnkPairArray field_0x50;
    UnkPairArray field_0x78;
    u64 field_0xa0;
};

void fn_7100dcff0c(UnkPairArrays* self, UnkPairArrays& other) {
    if (self == &other)
        return;
    self->field_0x0 = static_cast<UnkPairArray&&>(other.field_0x0);
    self->field_0x28 = static_cast<UnkPairArray&&>(other.field_0x28);
    self->field_0x50 = static_cast<UnkPairArray&&>(other.field_0x50);
    self->field_0x78 = static_cast<UnkPairArray&&>(other.field_0x78);
    self->field_0xa0 = other.field_0xa0;
}
