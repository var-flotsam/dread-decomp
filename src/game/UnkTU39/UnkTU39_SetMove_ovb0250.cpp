// Unit 39 of tools/tu_map.py (no reliable name): the move of a deferred callback set (the one the keyed lists
// keep at +0x30). The new set gets three arrays (growth step 10, flags 0xf) and the lock flag; the old set's
// callbacks are re-owned by the new one and appended to its items; the old set's counts are cleared.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::UnkVector;

void fn_710003017c(UnkDeferredSet<UnkDeferredCallback<f32>>* self, UnkDeferredSet<UnkDeferredCallback<f32>>* other) {
    new (&self->items) UnkVector<UnkDeferredCallback<f32>*>(0xf);
    new (&self->pending_add) UnkVector<UnkDeferredCallback<f32>*>(0xf);
    new (&self->pending_remove) UnkVector<UnkDeferredCallback<f32>*>(0xf);
    self->locked = other->locked;
    for (UnkDeferredCallback<f32>**it = other->items.data, **end = it + other->items.count; it != end; ++it) {
        UnkDeferredCallback<f32>* cb = *it;
        __builtin_memcpy(&cb->field_0x0, &self, sizeof(self));
        self->items.PushBack(cb);
    }
    other->items.count = 0;
    other->pending_add.count = 0;
    other->pending_remove.count = 0;
}
