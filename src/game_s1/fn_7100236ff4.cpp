// Iterator increments of two reflected arrays of refcounted pointers: step the index and element
// and re-point the cached value at them, releasing a value it owned (fn_7100236ff4 and
// fn_7100237494). fn_710011aa2c is the same for larger elements. Called only through their
// collection function tables.

#include "base/UnkCollectionIter.h"
#include "base/UnkDestructors.h"
#include "types.h"

using UnkRefPtrIter = base::UnkCollectionIter<base::UnkRefPtrMember>;

static inline void Increment(UnkRefPtrIter* it) {
    ++it->index;
    ++it->element;
    it->current.SetRef(&it->index, it->element);
}

void fn_7100236ff4(UnkRefPtrIter* it) { Increment(it); }

void fn_7100237494(UnkRefPtrIter* it) { Increment(it); }
