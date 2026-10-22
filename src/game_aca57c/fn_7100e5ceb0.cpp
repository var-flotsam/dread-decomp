// A pair's move constructor and its constructor from a moved key and a vector: the string id
// key is taken over (the source is left holding the empty id) and the vector of string ids is
// copied (base::UnkStrIdVec). Also the equality of iterator values over such entries.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkMovedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkIdVecEntry {
    UnkMovedStrId key;
    base::UnkStrIdVec ids;

    __attribute__((always_inline)) UnkIdVecEntry(UnkIdVecEntry&& other)
        : key(static_cast<UnkMovedStrId&&>(other.key)), ids(other.ids) {}
    __attribute__((always_inline)) UnkIdVecEntry(UnkMovedStrId&& key, const base::UnkStrIdVec& ids)
        : key(static_cast<UnkMovedStrId&&>(key)), ids(ids) {}
};

void fn_7100e5ceb0(UnkIdVecEntry* self, UnkIdVecEntry* other) {
    new (self) UnkIdVecEntry(static_cast<UnkIdVecEntry&&>(*other));
}
void fn_7100e5cfd0(UnkIdVecEntry* self, UnkMovedStrId* key, const base::UnkStrIdVec* ids) {
    new (self) UnkIdVecEntry(static_cast<UnkMovedStrId&&>(*key), *ids);
}

// Equality of two iterator values over such entries, or entries keyed by a word
// (base::UnkIterValue's layout, the state at 0x30): the keys and then the vectors' string ids are
// compared by identity.
template <class Key>
using UnkIdVecIterValue = base::UnkIterValue<base::UnkStrIdVec, Key>;

__attribute__((always_inline)) static inline bool KeyEqual(const UnkMovedStrId& a, const UnkMovedStrId& b) {
    return a.data == b.data;
}
__attribute__((always_inline)) static inline bool KeyEqual(const u32& a, const u32& b) { return a == b; }

template <class Key>
__attribute__((always_inline)) static inline bool Equal(const UnkIdVecIterValue<Key>* a, const UnkIdVecIterValue<Key>* b) {
    const Key* ka = a->state == 2 ? a->ref.index : &a->value.index;
    const Key* kb = b->state == 2 ? b->ref.index : &b->value.index;
    if (!KeyEqual(*ka, *kb))
        return false;
    const base::UnkStrIdVec* x = a->state == 2 ? a->ref.element : &a->value.element;
    const base::UnkStrIdVec* y = b->state == 2 ? b->ref.element : &b->value.element;
    if (x->count != y->count)
        return false;
    for (u64 i = 0; i < x->count; ++i)
        if (y->data[i].data != x->data[i].data)
            return false;
    return true;
}

// NON_MATCHING(minor): the first state and the first pointers swap registers (x8 and x9).
bool fn_7100e5c63c(const UnkIdVecIterValue<UnkMovedStrId>* a, const UnkIdVecIterValue<UnkMovedStrId>* b) {
    return Equal(a, b);
}
// NON_MATCHING(minor): as fn_7100e5c63c.
bool fn_7100e5cd28(const UnkIdVecIterValue<UnkMovedStrId>* a, const UnkIdVecIterValue<UnkMovedStrId>* b) {
    return Equal(a, b);
}
// NON_MATCHING(minor): as fn_7100e5c63c, and the first key is loaded after the second state's
// test.
bool fn_7100d42e94(const UnkIdVecIterValue<u32>* a, const UnkIdVecIterValue<u32>* b) { return Equal(a, b); }
// NON_MATCHING(minor): as fn_7100d42e94.
bool fn_7100d43390(const UnkIdVecIterValue<u32>* a, const UnkIdVecIterValue<u32>* b) { return Equal(a, b); }
