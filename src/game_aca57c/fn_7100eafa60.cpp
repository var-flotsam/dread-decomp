// Copy constructor of an array (base::UnkVector's layout) of string ids with two values each,
// through an out-of-line Reserve (fn_7100b48af4): takes the source's settings, reserves its
// capacity and appends each element, growing like PushBack (including its hang).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdValues {
    CStrId id;
    u32 field_0x8;
    u32 field_0xc;

    // The values are stored through plain pointers (the original reloads the array's count
    // after them).
    UnkIdValues(const UnkIdValues& other) : id(other.id) {
        u32* values = &field_0x8;
        values[0] = other.field_0x8;
        values[1] = other.field_0xc;
    }
};

struct UnkIdValuesArray {
    UnkIdValues* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100b48af4(UnkIdValuesArray* self, u32 capacity);

void fn_7100eafa60(UnkIdValuesArray* self, const UnkIdValuesArray& other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other.step;
    self->field_0x14 = other.field_0x14;
    self->flags = other.flags;
    self->allocator = other.allocator;
    fn_7100b48af4(self, other.capacity);
    for (const UnkIdValues *it = other.data, *end = it + other.count; it != end; ++it) {
        u32 n = self->count;
        u32 new_capacity = self->capacity;
        if (new_capacity <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100b48af4(self, new_capacity);
        new (&self->data[self->count]) UnkIdValues(*it);
        ++self->count;
    }
}
