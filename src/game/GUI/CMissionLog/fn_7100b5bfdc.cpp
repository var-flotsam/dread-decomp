// Copy constructor of an array (base::UnkVector's layout) of 0x38-byte elements through an
// out-of-line Reserve (fn_7100b5c28c) and push-back (fn_7100b593d0): takes the source's settings,
// reserves its capacity and appends each element.
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem38 {
    u8 unk_0x0[0x38];
};

struct UnkElem38Array {
    UnkElem38* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_7100b5c28c(UnkElem38Array* self, u32 capacity);
void fn_7100b593d0(UnkElem38Array* self, const UnkElem38* value);

void fn_7100b5bfdc(UnkElem38Array* self, const UnkElem38Array& other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other.step;
    self->field_0x14 = other.field_0x14;
    self->flags = other.flags;
    self->allocator = other.allocator;
    fn_7100b5c28c(self, other.capacity);
    for (const UnkElem38 *it = other.data, *end = it + other.count; it != end; ++it)
        fn_7100b593d0(self, it);
}
