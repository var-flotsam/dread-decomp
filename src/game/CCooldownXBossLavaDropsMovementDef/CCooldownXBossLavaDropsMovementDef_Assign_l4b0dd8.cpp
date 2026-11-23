// CCooldownXBossLavaDropsMovementDef's unit (tools/tu_map.py): the copy assignment of a
// base::global::CRntVector of 0x18-byte polymorphic elements (vtable unk_7101a3e068 and three words).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CCooldownXBossLavaDropsMovementDef.h"
#include "game/UnkLavaDropsData_l4b0dd8.h"
#include "game/UnkLavaDropsElemVtable_l4b0dd8.h"
#include "game/UnkLavaDropsElem_l4b0dd8.h"
#include "types.h"

struct UnkLavaDropsElem_l4b0dd8;

static_assert(sizeof(UnkLavaDropsElem_l4b0dd8) == 0x18, "UnkLavaDropsElem_l4b0dd8 layout");

using UnkLavaDropsVector_l4b0dd8 = base::global::CRntVector<UnkLavaDropsElem_l4b0dd8>;

UnkLavaDropsVector_l4b0dd8* fn_71004b0dd8(UnkLavaDropsVector_l4b0dd8* self, const UnkLavaDropsVector_l4b0dd8* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].~UnkLavaDropsElem_l4b0dd8();
        self->count = 0;
        self->Reserve(other->capacity);
        for (const UnkLavaDropsElem_l4b0dd8 *it = other->data, *end = other->data + other->count; it != end; ++it)
        {
            // CRntVector::PushBackCopy, inlined: grow to the next multiple of the step.
            u32 n = self->count;
            u32 new_capacity = self->capacity;
            if (new_capacity <= n && self->step != 0)
                new_capacity = (n / self->step + 1) * self->step;
            if ((s32)n >= (s32)new_capacity)
                for (;;) {
                }
            self->Reserve(new_capacity);
            new (&self->data[self->count]) UnkLavaDropsElem_l4b0dd8(*it);
            ++self->count;
        }
    }
    return self;
}
