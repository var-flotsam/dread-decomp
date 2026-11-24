// behaviortree::CBehaviorTree (unit 548): takes the first pending entry off the list and runs its
// node for one step; a finishing status (5 or 6, or 7) is reported to the node's listener, and
// any other status puts the entry back at the end of the list. False when the list is empty or
// the entry has no node. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

struct UnkBehaviorNodeView_l3aa8e0;

struct UnkListener_l3aa8e0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(UnkBehaviorNodeView_l3aa8e0* node, s32 state);
    virtual void vf_0x40(UnkBehaviorNodeView_l3aa8e0* node);
};

struct UnkListenerHolder_l3aa8e0 {
    u8 pad_0x0[0x10];
    UnkListener_l3aa8e0* listener;  // 0x10
};

// The node as this function reads it (its listener holder at +0x20).
struct UnkBehaviorNodeView_l3aa8e0 {
    u8 pad_0x0[0x20];
    UnkListenerHolder_l3aa8e0* holder;  // 0x20
};

struct UnkPendingNode_l3aa8e0 {
    virtual void vf_0x0();
    u32 state;
    UnkBehaviorNodeView_l3aa8e0* key;
    UnkPendingNode_l3aa8e0* next;

    UnkPendingNode_l3aa8e0(UnkBehaviorNodeView_l3aa8e0* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkBehaviorTree_l3aa8e0 {
    u8 pad_0x0[0x20];
    base::UnkLinkedList<UnkPendingNode_l3aa8e0> pending;  // 0x20
};

bool fn_71003aa8e0(UnkBehaviorTree_l3aa8e0* self, f32 dt) {
    UnkPendingNode_l3aa8e0* first = self->pending.head;
    if (!first)
        return false;
    UnkBehaviorNodeView_l3aa8e0* node = first->key;
    u32 state = first->state;
    self->pending.head = first->next;
    self->pending.allocator->vf_0x48(first);
    if (--self->pending.count == 0)
        self->pending.tail = self->pending.head;
    if (!node)
        return false;
    if (state - 1 <= 2)
        state = CBehavior_AdvanceStatus(reinterpret_cast<UnkBehaviorNode_A2_b69*>(node), dt, state);
    if (state - 5 <= 1) {
        if (UnkListenerHolder_l3aa8e0* holder = node->holder)
            if (holder->listener)
                holder->listener->vf_0x38(node, state);
    } else if (state == 7) {
        if (UnkListenerHolder_l3aa8e0* holder = node->holder)
            if (holder->listener)
                holder->listener->vf_0x40(node);
    } else {
        void* mem = self->pending.allocator->vf_0x40(sizeof(UnkPendingNode_l3aa8e0), 8, self->pending.flags);
        if (mem)
            self->pending.PushBack(new (mem) UnkPendingNode_l3aa8e0(node, state));
    }
    return true;
}
