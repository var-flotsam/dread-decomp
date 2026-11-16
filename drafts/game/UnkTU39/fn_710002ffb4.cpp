// Unit 39: removes the node holding a given string id from the list of src/base/UnkStrIdList.cpp,
// releases its id, frees it and tells the callbacks when the list becomes empty. Names and layouts
// are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkLinkedList.h"
#include "base/UnkDeferredSet.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU39.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;

struct UnkStrIdListNode {
    base::global::CStrId id;
    u64 field_0x8;
    UnkStrIdListNode* next;
};

struct UnkStrIdList {
    u64 field_0x0;
    base::UnkLinkedList<UnkStrIdListNode> list;  // 0x8
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x30
};

void fn_710002ffb4(UnkStrIdList* self, const base::global::CStrId* key) {
    u32 old_count = self->list.count;
    UnkStrIdListNode* found = self->list.head;
    while (found && found->id.data != key->data)
        found = found->next;
    if (found)
        self->list.Remove(found);
    bool was_filled = old_count != 0;
    bool arg = self->list.count != 0;
    if (was_filled != arg)
        fn_71000303c0(&self->on_change, &arg);
}
