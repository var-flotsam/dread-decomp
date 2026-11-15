// Unit 2702 (CAIComponent): the "set element at index" function of the callback-set array: an index
// past the end of a growable array reserves up to it, constructs the new elements and extends the
// count; the element at the index is then copy-assigned. Names and layouts are placeholders.
#include <new>

#include "base/UnkRefHandle.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkTU341.h"
#include "types.h"

static inline void Grow_l113df0(UnkCallbackSetVector* self, u32 index) {
    u32 capacity = self->capacity;
    if (capacity <= index && self->step != 0)
        capacity = (index / self->step + 1) * self->step;
    fn_71011143d8(self, capacity);
    u32 n = index + 1;
    fn_71011143d8(self, n);
    for (u32 i = self->count; i < n; ++i)
        new (&self->data[i]) UnkCallbackSetElem();
    self->count = n;
}

void fn_7101113df0(UnkCallbackSetVector* self, const u32* index, const UnkCallbackSetElem* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i)
        Grow_l113df0(self, i);
    UnkCallbackSetElem* e = &self->data[i];
    fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&e->set.owner),
                  reinterpret_cast<const base::UnkRefHandle*>(&value->set.owner));
    e->set.field_0x30 = value->set.field_0x30;
    fn_710023b29c(&e->set.callbacks, &value->set.callbacks);
    e->field_0x40 = value->field_0x40;
}
