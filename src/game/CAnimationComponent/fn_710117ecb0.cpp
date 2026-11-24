// CAnimationComponent's unit (tools/tu_map.py): ends a walk over the component's listeners (+0x438),
// a counted deferred set (base/UnkCountedDeferredSet.h; a copy of fn_710113831c): once the walk
// count is back to zero, the listeners removed meanwhile leave the set and those added join it.
// Names and layouts are placeholders.
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CRntVector.h"
#include "types.h"

// The element at `index`, growing the list when needed (an out-of-line CRntVector::GrowAt).
void** fn_710117ee74(base::global::CRntVector<void*>* list, u32 index);

void CAnimationComponent_EndListenersWalk(base::UnkCountedDeferredWalk<void>* walk) {
    --walk->set->lock;
    base::UnkCountedDeferredSet<void>* listeners = walk->set;
    if (listeners->lock > 0)
        return;
    for (u32 i = 0; i < listeners->pending_remove.count; ++i)
        listeners->items.Remove(*fn_710117ee74(&listeners->pending_remove, i));
    // Cleared through an untyped store: the original reloads the walk's pointer after it.
    __builtin_memset(&listeners->pending_remove.count, 0, sizeof(u32));
    listeners = walk->set;
    for (u32 i = 0; i < listeners->pending_add.count; ++i)
        listeners->items.PushBack(*fn_710117ee74(&listeners->pending_add, i));
    listeners->pending_add.count = 0;
}
