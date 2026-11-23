// CCooldownXBossFireBallDef's unit (tools/tu_map.py): the copy assignment of its array of 0x18-byte
// polymorphic elements (base/global/CRntVector.h): unless assigned to itself, destroy the elements,
// reserve the other's capacity, then append a copy of each element. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "game/CCooldownXBossFireBallDef.h"
#include "game/UnkFireBallDefData_ovb0072.h"
#include "game/UnkFireBallDefVtable_ovb0072.h"
#include "game/UnkFireBallDef_ovb0072.h"
#include "types.h"

struct UnkFireBallDef_ovb0072;

static_assert(sizeof(UnkFireBallDef_ovb0072) == 0x18, "UnkFireBallDef_ovb0072 layout");

using UnkFireBallDefVector_ovb0072 = base::global::CRntVector<UnkFireBallDef_ovb0072>;

UnkFireBallDefVector_ovb0072* fn_71004bd548(UnkFireBallDefVector_ovb0072* self, const UnkFireBallDefVector_ovb0072* other) {
    if (self != other) {
        self->Clear();
        self->Reserve(other->capacity);
        if (other->count != 0) {
            const UnkFireBallDef_ovb0072* end = other->data + other->count;
            for (const UnkFireBallDef_ovb0072* p = other->data; p != end; ++p) {
                // Grow to the next multiple of the step when full (a zero step hangs, as in the original).
                u32 n = self->count;
                u32 new_capacity = self->capacity;
                if (new_capacity <= n && self->step != 0)
                    new_capacity = (n / self->step + 1) * self->step;
                if ((s32)n >= (s32)new_capacity)
                    for (;;) {
                    }
                self->Reserve(new_capacity);
                new (&self->data[self->count]) UnkFireBallDef_ovb0072(*p);
                ++self->count;
            }
        }
    }
    return self;
}
