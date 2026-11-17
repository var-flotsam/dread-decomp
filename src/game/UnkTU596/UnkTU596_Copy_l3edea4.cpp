// Unit 596: copy-constructs an array of 12-byte records: CRntVector's copy constructor written out
// (take the source's settings and allocator, reserve its capacity, then append each record). Names
// and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

// Two words that start at zero and a value that starts at -1.
struct UnkKeyValue_l3edea4 {
    f32 field_0x0 = 0.0f;
    f32 field_0x4 = 0.0f;
    f32 value = -1.0f;

    ~UnkKeyValue_l3edea4() {}
};

using UnkKeyValueVector_l3edea4 = base::global::CRntVector<UnkKeyValue_l3edea4>;

void fn_71003edea4(UnkKeyValueVector_l3edea4* self, const UnkKeyValueVector_l3edea4* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkKeyValue_l3edea4 *it = other->data, *end = it + other->count; it != end; ++it) {
        // Append, growing to the next multiple of the step when full.
        u32 n = self->count;
        u32 cap = self->capacity;
        u32 new_capacity = cap;
        if (cap <= n && self->step != 0)
            new_capacity = (n / self->step + 1) * self->step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        if (cap < new_capacity) {
            self->Reserve(new_capacity);
            n = self->count;
        }
        new (&self->data[n]) UnkKeyValue_l3edea4(*it);
        ++self->count;
    }
}
