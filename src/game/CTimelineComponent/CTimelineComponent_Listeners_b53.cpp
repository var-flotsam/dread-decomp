// CTimelineComponent unit (tools/tu_map.py, the name from reflection): a copy of fn_710117eb1c
// (src/game/CAnimationComponent/CAnimationComponent_b52.cpp). Names and layouts are placeholders.
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CRntVector.h"
#include "types.h"

// Removes the first `value` from the array (data, count), shifting the rest down.
static inline __attribute__((always_inline)) void RemoveFirst(base::global::CRntVector<void*>& v, void* value) {
    u32 count = v.count;
    if (count == 0)
        return;
    void** data = v.data;
    for (u32 i = 0; i < count; ++i) {
        if (data[i] == value) {
            __builtin_memcpy(&data[i], &data[i + 1], (u64)(count - i - 1) * sizeof(void*));
            --v.count;
            return;
        }
    }
}

// Removes `value` from the component's listeners: during a walk it is recorded as removed and taken
// out of the added ones; otherwise it is taken out of the set.
void fn_7100a2e30c(base::UnkCountedDeferredSet<void>* self, void* value) {
    if (self->lock <= 0) {
        RemoveFirst(self->items, value);
    } else {
        self->pending_remove.PushBack(value);
        RemoveFirst(self->pending_add, value);
    }
}
