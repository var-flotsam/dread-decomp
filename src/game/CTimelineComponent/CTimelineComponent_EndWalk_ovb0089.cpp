// CTimelineComponent unit (tools/tu_map.py): ends a walk over the component's listeners, a counted
// deferred set (base/UnkCountedDeferredSet.h; a copy of fn_710113831c,
// src/game/CAttackComponent/CAttackComponent_Targets_ovb0050.cpp): once the walk count is back to
// zero, the listeners removed meanwhile leave the set and those added join it. Names and layouts are
// placeholders.
#include "base/UnkCountedDeferredSet.h"
#include "base/global/CRntVector.h"
#include "types.h"

using UnkTimelineListeners = base::UnkCountedDeferredSet<void>;

void** fn_7100a2e664(base::global::CRntVector<void*>* self, u32 index);

void CTimelineComponent_EndListenerWalk(base::UnkCountedDeferredWalk<void>* walk) {
    --walk->set->lock;
    UnkTimelineListeners* lists = walk->set;
    if (lists->lock > 0)
        return;
    for (u32 i = 0; i < lists->pending_remove.count; ++i)
        lists->items.Remove(*fn_7100a2e664(&lists->pending_remove, i));
    __builtin_memset(&lists->pending_remove.count, 0, sizeof(u32));
    lists = walk->set;
    for (u32 i = 0; i < lists->pending_add.count; ++i)
        lists->items.PushBack(*fn_7100a2e664(&lists->pending_add, i));
    lists->pending_add.count = 0;
}