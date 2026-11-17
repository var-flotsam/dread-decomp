// CAttackComponent unit (tools/tu_map.py unit 2706): the component's targets, a counted deferred set
// (base/UnkCountedDeferredSet.h). fn_7101138188 removes an entry: during a walk it records it as
// removed and takes it out of the added ones; otherwise it takes it out of the set. fn_710113831c
// ends a walk: once the count is back to zero, the removed entries leave the set and the added ones
// join it, and both records are emptied. Elements are read through the array's out-of-line accessor
// fn_71011384e0. Names and layouts are placeholders.
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CRntVector.h"
#include "types.h"

using UnkAttackTargets = base::UnkCountedDeferredSet<void>;

// base::global::CRntVector<void*>::GrowAt, out of line.
void** fn_71011384e0(base::global::CRntVector<void*>* self, u32 index);

void fn_7101138188(UnkAttackTargets* self, void* target) {
    if (self->lock <= 0) {
        self->items.Remove(target);
        return;
    }
    self->pending_remove.PushBack(target);
    self->pending_add.Remove(target);
}

void fn_710113831c(base::UnkCountedDeferredWalk<void>* walk) {
    --walk->set->lock;
    UnkAttackTargets* targets = walk->set;
    if (targets->lock > 0)
        return;
    for (u32 i = 0; i < targets->pending_remove.count; ++i)
        targets->items.Remove(*fn_71011384e0(&targets->pending_remove, i));
    // Cleared through an untyped store: the original reloads the walk's pointer after it.
    __builtin_memset(&targets->pending_remove.count, 0, sizeof(u32));
    targets = walk->set;
    for (u32 i = 0; i < targets->pending_add.count; ++i)
        targets->items.PushBack(*fn_71011384e0(&targets->pending_add, i));
    targets->pending_add.count = 0;
}