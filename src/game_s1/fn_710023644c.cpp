// Iterator increments of two reflected linked lists of 8-byte values: step the index, follow the
// node's link (at 8) and re-point the cached value at the new node, or reset it at the end
// (fn_710023644c, fn_71002367c8). fn_7100236938 is a relative. Called only through their
// collection function tables.

#include "base/UnkCollectionIter.h"
#include "types.h"

struct UnkListNode_710023644c {
    u64 value;
    UnkListNode_710023644c* next;
};

struct UnkListIter_710023644c {
    UnkListNode_710023644c* node;
    u32 index;
    base::UnkIterValue<u64> current;
};

static inline void Increment(UnkListIter_710023644c* it) {
    ++it->index;
    it->node = it->node->next;
    if (it->node)
        it->current.SetRef(&it->index, &it->node->value);
    else
        it->current.Reset();
}

void fn_710023644c(UnkListIter_710023644c* it) { Increment(it); }

void fn_71002367c8(UnkListIter_710023644c* it) { Increment(it); }
