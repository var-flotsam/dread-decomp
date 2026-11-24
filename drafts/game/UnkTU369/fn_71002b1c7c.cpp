// Unit 369 (tools/tu_map.py): adds an object to the list for its cell unless it is there already,
// then tells the listeners for added objects. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CRntVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CRntVector;

struct UnkClone_fn_71002b1eac;
using UnkTrackerListeners = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_71002b1eac, u64>(UnkTrackerListeners*, const u64*);

UnkOwnedList* GrowOwnedListArrayAndGetListAt(base::global::CRntVector<UnkOwnedList>* self, u32 index);

struct UnkTrackedObject {
    u16 field_0x0;
    u16 cell;  // 0x2
};

struct UnkObjectTracker {
    CRntVector<UnkOwnedList> cells;  // 0x0
    UnkTrackerListeners added;       // 0x28
    UnkTrackerListeners removed;     // 0xa8
};
static_assert(__builtin_offsetof(UnkObjectTracker, removed) == 0xa8, "layout");

void fn_71002b1c7c(UnkObjectTracker* self, UnkTrackedObject* object) {
    u64 key = reinterpret_cast<u64>(object);
    if (!object)
        return;
    CRntVector<u64>* list = reinterpret_cast<CRntVector<u64>*>(GrowOwnedListArrayAndGetListAt(&self->cells, object->cell));
    u32 n = list->count;
    u64* values = list->data;
    u32 index = 0;
    for (; index < n; ++index)
        if (values[index] == key)
            break;
    if (index != n)
        return;
    list->PushBack(key);
    base::refl_DeferredInvoke<UnkClone_fn_71002b1eac, u64>(&self->added, &key);
}
