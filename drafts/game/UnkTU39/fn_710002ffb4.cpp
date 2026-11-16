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

// base::UnkLinkedList::Remove, destroying the node (releasing its id) before freeing it.
template <class Node>
static inline void RemoveAndDestroy(base::UnkLinkedList<Node>* list, Node* node) {
    Node* prev = nullptr;
    for (Node* cur = list->head; cur;) {
        Node* next = cur->next;
        if (cur == node) {
            if (prev) {
                prev->next = next;
                if (list->tail == node)
                    list->tail = prev;
            } else {
                list->head = next;
            }
            if (--list->count == 0) {
                list->head = nullptr;
                list->tail = nullptr;
            }
            node->~Node();
            list->allocator->vf_0x48(node);
            return;
        }
        prev = cur;
        cur = next;
    }
}

void fn_710002ffb4(UnkStrIdList* self, const base::global::CStrId* key) {
    u32 old_count = self->list.count;
    UnkStrIdListNode* found = self->list.head;
    while (found && found->id.data != key->data)
        found = found->next;
    if (found)
        RemoveAndDestroy(&self->list, found);
    bool was_filled = old_count != 0;
    bool arg = self->list.count != 0;
    if (was_filled != arg)
        fn_71000303c0(&self->on_change, &arg);
}
