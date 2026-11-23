// CCooldownXBossFireBallDef's unit (tools/tu_map.py): the copy assignment of its array of 0x18-byte
// polymorphic elements (base/global/CRntVector.h): unless assigned to itself, destroy the elements,
// reserve the other's capacity, then append a copy of each element.
#include "base/global/CRntVector.h"
#include "reflected/CCooldownXBossFireBallDef.h"
#include "types.h"

using FireBallDefVector = base::global::CRntVector<CCooldownXBossFireBallDef>;

FireBallDefVector* fn_71004bd548(FireBallDefVector* self, const FireBallDefVector* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        if (other->count != 0) {
            const CCooldownXBossFireBallDef* end = other->data + other->count;
            for (const CCooldownXBossFireBallDef* p = other->data; p != end; ++p) {
                // Grow to the next multiple of the step when full (a zero step hangs, as in the original).
                u32 n = self->count;
                u32 new_capacity = self->capacity;
                if (new_capacity <= n && self->step != 0)
                    new_capacity = (n / self->step + 1) * self->step;
                if ((s32)n >= (s32)new_capacity)
                    for (;;) {
                    }
                self->Reserve(new_capacity);
                new (&self->data[self->count]) CCooldownXBossFireBallDef(*p);
                ++self->count;
            }
        }
    }
    return self;
}
