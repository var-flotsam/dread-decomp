// Unit 39: removes the node holding a given string id from the list of src/base/UnkStrIdList.cpp,
// releases its id, frees it and tells the callbacks when the list becomes empty. Names and layouts
// are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
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
    // A union member so the stores below force count to be reloaded, as the original does.
    union {
        u32 count;
    };
    UnkStrIdListNode* head;
    UnkStrIdListNode* tail;
    base::UnkAllocator* allocator;
    u64 field_0x28;
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x30
};

void fn_710002ffb4(UnkStrIdList* self, const base::global::CStrId* key) {
    u32 old_count = self->count;
    u32 new_count = old_count;
    UnkStrIdListNode* found = self->head;
    while (found && found->id.data != key->data)
        found = found->next;
    if (found) {
        UnkStrIdListNode* prev = nullptr;
        UnkStrIdListNode* node = self->head;
        UnkStrIdListNode* next;
        for (;;) {
            if (!node)
                goto done;
            next = node->next;
            if (node == found)
                break;
            prev = node;
            node = next;
        }
        if (prev) {
            prev->next = next;
            if (self->tail == found)
                self->tail = prev;
        } else {
            self->head = next;
        }
        if (--self->count == 0) {
            self->head = nullptr;
            self->tail = nullptr;
        }
        found->id.Reset();
        self->allocator->vf_0x48(found);
        new_count = self->count;
    }
done:
    bool was_filled = old_count != 0;
    bool arg = new_count != 0;
    if (was_filled != arg)
        fn_71000303c0(&self->on_change, &arg);
}
