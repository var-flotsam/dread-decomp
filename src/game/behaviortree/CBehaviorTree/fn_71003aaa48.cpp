// behaviortree::CBehaviorTree (unit 548): pushes a pending entry with state 1 and the tree's
// object (+0x88) at the front of its list of pending entries. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
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
    base::UnkLinkedList<UnkPendingNode_l3aaa48> pending;  // 0x20
    u8 pad_0x48[0x88 - 0x48];
    void* owner;  // 0x88
};

void fn_71003aaa48(UnkBehaviorTree_l3aaa48* self) {
    void* owner = self->owner;
    void* mem = self->pending.allocator->vf_0x40(sizeof(UnkPendingNode_l3aaa48), 8, self->pending.flags);
    if (mem)
        self->pending.PushFront(new (mem) UnkPendingNode_l3aaa48(owner, 1));
}
