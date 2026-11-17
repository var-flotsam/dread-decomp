#pragma once

// A set of pointers walked under a count: while the count is positive (a walk is in progress),
// additions and removals are recorded in the first two arrays and applied to the third when the
// last walk ends. Components keep their listeners in one (CAttackComponent at +0xa0,
// CTimelineComponent at +0xf8, CGunComponent at +0x228, CAnimationComponent at +0x438), and each
// has its own out-of-line copies of the operations: Add (refl_CountedDeferredAdd,
// base/UnkCloneHelpers.h; fn_7101138630, fn_710117efc4 and others), Remove (fn_7101138188,
// fn_710117eb1c, fn_7100a2e30c, fn_7100841838, fn_71010ce798) and the end of a walk
// (fn_710113831c, fn_710117ecb0, fn_7100a2e4a0, fn_7100841c4c). Names are placeholders.

#include "base/global/CRntVector.h"
#include "types.h"

namespace base {

template <class T>
struct UnkCountedDeferredSet {
    global::CRntVector<T*> pending_add;     // 0x0
    global::CRntVector<T*> pending_remove;  // 0x28
    global::CRntVector<T*> items;           // 0x50
    // The walk count. A union member (no type-based alias info): the original reloads the walk's
    // pointer to the set and the count after the decrement that ends a walk, and the array after
    // the increment that begins one.
    union {
        s32 lock;  // 0x78
    };
};
static_assert(__builtin_offsetof(UnkCountedDeferredSet<void>, lock) == 0x78, "UnkCountedDeferredSet layout");
static_assert(sizeof(UnkCountedDeferredSet<void>) == 0x80, "UnkCountedDeferredSet layout");

// A walk over a set: begun by raising its count, ended by the instance's out-of-line function.
template <class T>
struct UnkCountedDeferredWalk {
    UnkCountedDeferredSet<T>* set;

    explicit UnkCountedDeferredWalk(UnkCountedDeferredSet<T>* s) : set(s) { ++s->lock; }
};

}  // namespace base
