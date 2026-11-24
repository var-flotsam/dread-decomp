// Unit 1135: sets the entry for `key` in a list of (key, value) pairs kept in descending order of
// value: removes the old entry for the key, then inserts the new one before the first entry whose
// value is not greater. Names and layouts are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkClone_fn_710070f3b8;
using UnkPairList_l70f098 = base::UnkSList<UnkClone_fn_710070f3b8>;

extern template void UnkPairList_l70f098::InsertBefore(const u64&, UnkPairList_l70f098::Node*);

struct UnkOwner_710070f098 : UnkPairList_l70f098 {};

// What a node's 8-byte value holds; the list's out-of-line insert (shared with lists of plain
// 8-byte values) takes it as one.
struct UnkPair_l70f098 {
    u32 key;
    u32 value;
};

// The 8-byte value of a node: the key in the low word, the value in the high word.
static inline u32 KeyOf(const UnkPairList_l70f098::Node* n) {
    return static_cast<u32>(n->value);
}

static inline u32 ValueOf(const UnkPairList_l70f098::Node* n) {
    return static_cast<u32>(n->value >> 32);
}

void SetKeyEntryInListSortedByValueDesc(UnkOwner_710070f098* self, u32 key, u32 value) {
    UnkPairList_l70f098::Node* n = self->head;
    for (; n; n = n->next) {
        if (KeyOf(n) == key)
            break;
    }
    if (n)
        self->Remove(n);

    UnkPair_l70f098 pair = {key, value};
    const u64& entry = reinterpret_cast<const u64&>(pair);
    if (self->count == 0) {
        self->PushFront(entry);
        return;
    }
    for (n = self->head; n; n = n->next) {
        if (ValueOf(n) <= value) {
            self->InsertBefore(entry, n);
            return;
        }
    }
    self->PushBack(entry);
}
