// UnkTU2739 (unit 2739): drops every queued item whose resource has the given id from the manager's
// queue at +0xc8, freeing its node and handing the item to fn_71011a91c4 without deferring.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2739.h"
#include "types.h"

struct UnkRes_l1aa898 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x8];
    u64 id;
};

struct UnkItem_l1aa898 {
    void* vtable;
    u8 pad_0x8[0x8];
    UnkRes_l1aa898* res;
};

struct UnkNode_l1aa898 {
    UnkItem_l1aa898* item;
    UnkNode_l1aa898* next;
};

struct UnkQueue_l1aa898 {
    u8 pad_0x0[0xc8];
    u32 count;
    UnkNode_l1aa898* head;
    UnkNode_l1aa898* tail;
    base::UnkAllocator* allocator;
};

static inline UnkRes_l1aa898* AsRes(UnkRes_l1aa898* res) {
    return fn_710009d50c(res->vf_0x0(), fn_71000e8a5c()) ? res : nullptr;
}

// Unlinks and frees the node; returns the node after it, or null when it is not in the queue.
static inline UnkNode_l1aa898* Remove(UnkQueue_l1aa898* self, UnkNode_l1aa898* node) {
    UnkNode_l1aa898* prev = nullptr;
    UnkNode_l1aa898* cur = self->head;
    UnkNode_l1aa898* next;
    UnkNode_l1aa898* last = nullptr;
    for (;;) {
        if (!cur)
            return nullptr;
        prev = last;
        next = cur->next;
        if (cur == node)
            break;
        last = cur;
        cur = next;
    }
    if (prev) {
        prev->next = next;
        if (self->tail == node)
            self->tail = prev;
    } else {
        self->head = next;
    }
    if (--self->count == 0) {
        self->head = nullptr;
        self->tail = nullptr;
    }
    self->allocator->vf_0x48(node);
    return next;
}

void fn_71011aa898(UnkModelManager* manager, u64* model) {
    UnkQueue_l1aa898* self = reinterpret_cast<UnkQueue_l1aa898*>(manager);
    UnkNode_l1aa898* node = self->head;
    while (node) {
        if (AsRes(node->item->res)->id != *model) {
            node = node->next;
            continue;
        }
        UnkItem_l1aa898* item = node->item;
        UnkNode_l1aa898* next = Remove(self, node);
        fn_71011a91c4(manager, item, 0);
        node = next;
    }
}
