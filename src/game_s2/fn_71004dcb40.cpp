// Copy constructor of a growable array of 0x50-byte elements: takes over the other array's step,
// flag, flags and allocator, reserves its capacity and appends a copy of each element.
#include "game_s2/UnkItem50.h"

struct UnkRange {
    const UnkItem* data;
    u32 count;
    const UnkItem* begin() const { return data; }
    const UnkItem* end() const { return data + count; }
};

// NON_MATCHING(minor): the three zero stores of a moved element (0x34, 0x3c, 0x44) are emitted in
// ascending order, the original has the first and last swapped
void fn_71004dcb40(base::UnkVector<UnkItem>* self, const base::UnkVector<UnkItem>* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkItem& item : UnkRange{other->data, other->count})
        fn_71004dccac(self, &item);
}
