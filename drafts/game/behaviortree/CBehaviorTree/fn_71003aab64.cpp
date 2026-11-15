// behaviortree::CBehaviorTree (unit 548): pushes a pending entry with state 1 for `key` at the
// front of the list of pending entries; false for a null key. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPendingNode_l3aab64 {
    virtual void vf_0x0();
    u32 state;
    void* key;
    UnkPendingNode_l3aab64* next;

    UnkPendingNode_l3aab64(void* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkBehaviorTree_l3aab64 {
    u8 pad_0x0[0x20];
    union {  // no type-based alias info: reloaded after the node's link is stored
        u32 count;
    };
    UnkPendingNode_l3aab64* head;
    UnkPendingNode_l3aab64* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void PushFront(UnkPendingNode_l3aab64* node) {
        if (count) {
            node->next = head;
            head = node;
        } else {
            head = node;
            tail = node;
        }
        ++count;
    }
};

bool fn_71003aab64(UnkBehaviorTree_l3aab64* self, void* key) {
    if (!key)
        return false;
    void* mem = self->allocator->vf_0x40(sizeof(UnkPendingNode_l3aab64), 8, self->flags);
    if (mem)
        self->PushFront(new (mem) UnkPendingNode_l3aab64(key, 1));
    return true;
}
