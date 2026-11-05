// CTimelineComponent unit (tools/tu_map.py, the name from reflection): a copy of fn_710117eb1c
// (src/game/CAnimationComponent/CAnimationComponent_b52.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

// Three pointer lists and a lock count (+0x78).
struct UnkTimelineListeners_b53 {
    base::UnkVector<void*> active;   // 0x0
    base::UnkVector<void*> pending;  // 0x28
    base::UnkVector<void*> queued;   // 0x50
    s32 lock;                        // 0x78
};

// Removes the first `value` from the array (data, count), shifting the rest down.
static inline __attribute__((always_inline)) void RemoveFirst(base::UnkVector<void*>& v, void* value) {
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

// While locked, appends `value` to the pending list and takes it out of the active one; otherwise
// takes it out of the queued list.
void fn_7100a2e30c(UnkTimelineListeners_b53* self, void* value) {
    if (self->lock <= 0) {
        RemoveFirst(self->queued, value);
    } else {
        self->pending.PushBack(value);
        RemoveFirst(self->active, value);
    }
}
