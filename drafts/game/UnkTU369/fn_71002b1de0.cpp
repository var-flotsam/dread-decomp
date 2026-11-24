// Unit 369 (tools/tu_map.py): takes an object out of the list for its cell, then tells the
// removal listeners. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CRntVector.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

using base::global::CRntVector;

struct UnkClone_fn_71002b1eac;
using UnkRemovalListeners_l2b1de0 = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_71002b1eac, u64>(UnkRemovalListeners_l2b1de0*,
                                                                              const u64*);

UnkOwnedList* GrowOwnedListArrayAndGetListAt(base::global::CRntVector<UnkOwnedList>* self, u32 index);

struct UnkTrackedObject_l2b1de0 {
    u16 field_0x0;
    u16 cell;  // 0x2
};

struct UnkObjectTracker_l2b1de0 {
    CRntVector<UnkOwnedList> cells;  // 0x0
    u8 pad_0x28[0x80];
    UnkRemovalListeners_l2b1de0 removed;  // 0xa8
};
static_assert(__builtin_offsetof(UnkObjectTracker_l2b1de0, removed) == 0xa8, "layout");

using UnkObjectList_l2b1de0 = CRntVector<UnkTrackedObject_l2b1de0*>;

void fn_71002b1de0(UnkObjectTracker_l2b1de0* self, UnkTrackedObject_l2b1de0* object) {
    u64 arg = reinterpret_cast<u64>(object);
    if (!object)
        return;
    UnkObjectList_l2b1de0* list = reinterpret_cast<UnkObjectList_l2b1de0*>(GrowOwnedListArrayAndGetListAt(&self->cells, object->cell));
    UnkTrackedObject_l2b1de0** values = list->data;
    u32 n = list->count;
    u64 index = 0;
    for (; index < n; ++index)
        if (values[index] == object)
            break;
    if (index == list->count)
        return;
    // Shift the later entries down over it.
    if ((u32)index < n) {
        memcpy(&values[index], &values[index + 1], (u64)(n - (u32)index - 1) * sizeof(*values));
        --list->count;
    }
    base::refl_DeferredInvoke<UnkClone_fn_71002b1eac, u64>(&self->removed, &arg);
}
