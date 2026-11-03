// From unit 1115 of tools/tu_map.py (0x71006f8120-0x71006fbadc, no reflected class): the copy of an
// array of arrays. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// An inner array: base::UnkVector's layout with a move that leaves the source empty.
struct UnkInnerVec {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkInnerVec(UnkInnerVec&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

typedef base::UnkVector<UnkInnerVec> UnkOuterVec;

// Appends a copy of an inner array.
void fn_710056041c(UnkOuterVec* self, const UnkInnerVec* value);

// Copy-constructs `self` from `other`: the same settings and allocator, the capacity reserved up
// front, then each inner array appended.
void fn_71006fb3f4(UnkOuterVec* self, const UnkOuterVec* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkInnerVec *p = other->data, *end = p + other->count; p != end; ++p)
        fn_710056041c(self, p);
}
