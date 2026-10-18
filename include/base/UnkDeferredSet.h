#pragma once

// A set of pointers whose changes are deferred while it is locked (iterated): additions and
// removals then go to pending lists (fn_71002aeebc, fn_71002ae87c and their copies). Names
// are placeholders.

#include "base/UnkVector.h"
#include "types.h"

namespace base {

template <class T>
struct UnkDeferredSet {
    UnkVector<T*> items;
    UnkVector<T*> pending_add;
    UnkVector<T*> pending_remove;
    bool locked;

    void Add(T* item) {
        if (!locked)
            items.PushBack(item);
        else
            pending_add.PushBack(item);
    }

    void Remove(T* item) {
        if (!locked) {
            items.Remove(item);
        } else {
            pending_remove.PushBack(item);
            pending_add.Remove(item);
        }
    }
};
static_assert(sizeof(UnkDeferredSet<void>) == 0x80, "UnkDeferredSet layout");

}  // namespace base
