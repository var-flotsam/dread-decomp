// behaviortree::CBehaviorTree (unit 548): pushes a pending entry with state 1 and the tree's
// object (+0x88) at the front of its list of pending entries. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPendingNode_l3aaa48 {
    virtual void vf_0x0();
    u32 state;
    void* key;
    UnkPendingNode_l3aaa48* next;

    UnkPendingNode_l3aaa48(void* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkBehaviorTree_l3aaa48 {
    u8 pad_0x0[0x20];
    union {  // no type-based alias info: reloaded after the node's link is stored
        u32 count;
    };
    UnkPendingNode_l3aaa48* head;
    UnkPendingNode_l3aaa48* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    u8 pad_0x44[0x88 - 0x44];
    void* owner;  // 0x88

    void PushFront(UnkPendingNode_l3aaa48* node) {
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

void fn_71003aaa48(UnkBehaviorTree_l3aaa48* self) {
    void* owner = self->owner;
    void* mem = self->allocator->vf_0x40(sizeof(UnkPendingNode_l3aaa48), 8, self->flags);
    if (mem)
        self->PushFront(new (mem) UnkPendingNode_l3aaa48(owner, 1));
}
