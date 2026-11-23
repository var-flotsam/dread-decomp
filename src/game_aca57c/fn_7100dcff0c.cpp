// Move-assignment of four arrays of two parallel value lists (sharing a count and capacity) and
// a trailing value: each array takes over the other's storage and settings, leaving it empty.
#include "base/global/CRntVector.h"
#include "types.h"

using base::UnkAllocator;

struct UnkPairArray_dcff0c {
    void* values;
    void* ids;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;

    UnkPairArray_dcff0c& operator=(UnkPairArray_dcff0c&& other) {
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
    UnkPairArray_dcff0c field_0x0;
    UnkPairArray_dcff0c field_0x28;
    UnkPairArray_dcff0c field_0x50;
    UnkPairArray_dcff0c field_0x78;
    u64 field_0xa0;
};

void fn_7100dcff0c(UnkPairArrays* self, UnkPairArrays& other) {
    if (self == &other)
        return;
    self->field_0x0 = static_cast<UnkPairArray_dcff0c&&>(other.field_0x0);
    self->field_0x28 = static_cast<UnkPairArray_dcff0c&&>(other.field_0x28);
    self->field_0x50 = static_cast<UnkPairArray_dcff0c&&>(other.field_0x50);
    self->field_0x78 = static_cast<UnkPairArray_dcff0c&&>(other.field_0x78);
    self->field_0xa0 = other.field_0xa0;
}
