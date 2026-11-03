// game::logic::collision::CCollider unit (tools/tu_map.py, the name from reflection): the copy of an
// array of arrays, the same code as fn_71006fb3f4 (unit 1115) with this unit's append
// (fn_7100d43e70). Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// An inner array: base::UnkVector's layout with a move that leaves the source empty.
struct UnkInnerVec_col {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkInnerVec_col(UnkInnerVec_col&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    // Copies another inner array of 8-byte elements: the capacity reserved, then the elements.
    UnkInnerVec_col(const UnkInnerVec_col& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (n != 0) {
            void* p = allocator->vf_0x40((u64)n * 8, 8, flags);
            if (data) {
                __builtin_memcpy(p, data, (u64)count * 8);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * 8);
        count = other.count;
    }
};

typedef base::UnkVector<UnkInnerVec_col> UnkOuterVec_col;

// Appends a copy of an inner array.
void fn_7100d43e70(UnkOuterVec_col* self, const UnkInnerVec_col* value) {
    self->PushBackCopy(*value);
}

// Copy-constructs `self` from `other`: the same settings and allocator, the capacity reserved up
// front, then each inner array appended.
void fn_7100d43d50(UnkOuterVec_col* self, const UnkOuterVec_col* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkInnerVec_col *p = other->data, *end = p + other->count; p != end; ++p)
        fn_7100d43e70(self, p);
}
