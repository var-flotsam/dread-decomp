// A list of string ids from unit 39 of tools/tu_map.py (low confidence; no class hint): nodes of
// {id, a word, next} from an allocator, with a count, head and tail, and a deferred callback set at
// +0x30 told when the list is emptied (src/game_s1/fn_71000303c0.cpp). Holds the clear
// (fn_710002fa90). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;

void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg);

struct UnkStrIdListNode {
    base::global::CStrId id;
    u64 field_0x8;
    UnkStrIdListNode* next;
};

struct UnkStrIdList {
    u64 field_0x0;
    u32 count;
    UnkStrIdListNode* head;
    UnkStrIdListNode* tail;
    base::UnkAllocator* allocator;
    u64 field_0x28;
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x30
};

// Releases every id, frees the nodes and empties the list; when it was not empty, tells the
// callbacks (with false).
void fn_710002fa90(void* list) {
    UnkStrIdList* self = static_cast<UnkStrIdList*>(list);
    UnkStrIdListNode* node = self->head;
    u32 count = self->count;
    if (node) {
        UnkStrIdListNode* next = node->next;
        for (;;) {
            node->id.Reset();
            self->allocator->vf_0x48(node);
            if (!next)
                break;
            node = next;
            next = next->next;
        }
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    if (count != 0) {
        bool arg = false;
        fn_71000303c0(&self->on_change, &arg);
    }
}
