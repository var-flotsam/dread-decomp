// Copy assignment (returns this) of a growable array of 0x50-byte elements: destroys the current
// elements, reserves the other array's capacity and appends a copy of each of its elements (an
// assignment to itself changes nothing).
#include "game_s2/UnkItem50.h"

struct UnkRange {
    const UnkItem* data;
    u32 count;
    const UnkItem* begin() const { return data; }
    const UnkItem* end() const { return data + count; }
};

// NON_MATCHING(minor): the three zero stores of a moved element (0x34, 0x3c, 0x44) are emitted in
// ascending order, the original has the first and last swapped
base::UnkVector<UnkItem>* fn_71004dce50(base::UnkVector<UnkItem>* self, const base::UnkVector<UnkItem>* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        for (const UnkItem& item : UnkRange{other->data, other->count})
            fn_71004dccac(self, &item);
    }
    return self;
}
