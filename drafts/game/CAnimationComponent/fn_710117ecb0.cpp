// CAnimationComponent's unit (tools/tu_map.py): releases a lock on the component's listener lists;
// releasing the last one drops the pending listeners from the queued list and queues the active ones.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

// The component's listener lists (+0x438), as CAnimationComponent_Tracks_ovb0009.cpp defines them.
struct UnkAnimListeners {
    base::global::CRntVector<void*> active;   // 0x0
    base::global::CRntVector<void*> pending;  // 0x28
    base::global::CRntVector<void*> queued;   // 0x50
    union {
        s32 lock;  // 0x78
    };
};

struct UnkAnimListenersLock {
    UnkAnimListeners* listeners;

    explicit UnkAnimListenersLock(UnkAnimListeners* l);
    ~UnkAnimListenersLock();
};

// The element at `index`, growing the list when needed (an out-of-line CRntVector::GrowAt).
void** fn_710117ee74(base::global::CRntVector<void*>* list, u32 index);

void fn_710117ecb0(UnkAnimListenersLock* self) {
    --self->listeners->lock;
    UnkAnimListeners* listeners = self->listeners;
    if (listeners->lock > 0)
        return;

    for (u32 i = 0; i < listeners->pending.count; ++i)
        listeners->queued.Remove(*fn_710117ee74(&listeners->pending, i));
    listeners->pending.count = 0;

    listeners = self->listeners;
    for (u32 i = 0; i < listeners->active.count; ++i)
        listeners->queued.PushBack(*fn_710117ee74(&listeners->active, i));
    listeners->active.count = 0;
}
