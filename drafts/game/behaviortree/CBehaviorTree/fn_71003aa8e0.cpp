// behaviortree::CBehaviorTree (unit 548): takes the first pending entry off the list and runs its
// node for one step; a finishing status (5 or 6, or 7) is reported to the node's listener, and
// any other status puts the entry back at the end of the list. False when the list is empty or
// the entry has no node. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkBehaviorNode_A2_b69;
s32 fn_7100394b30(UnkBehaviorNode_A2_b69* node, f32 value, s32 state);

struct UnkListener_l3aa8e0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(UnkBehaviorNode_A2_b69* node, s32 state);
    virtual void vf_0x40(UnkBehaviorNode_A2_b69* node);
};

struct UnkListenerHolder_l3aa8e0 {
    u8 pad_0x0[0x10];
    UnkListener_l3aa8e0* listener;  // 0x10
};

struct UnkBehaviorNode_A2_b69 {
    u8 pad_0x0[0x20];
    UnkListenerHolder_l3aa8e0* holder;  // 0x20
};

struct UnkPendingNode_l3aa8e0 {
    virtual void vf_0x0();
    u32 state;
    UnkBehaviorNode_A2_b69* key;
    UnkPendingNode_l3aa8e0* next;

    UnkPendingNode_l3aa8e0(UnkBehaviorNode_A2_b69* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkBehaviorTree_l3aa8e0 {
    u8 pad_0x0[0x20];
    union {  // no type-based alias info: reloaded after the node's link is stored
        u32 count;
    };
    UnkPendingNode_l3aa8e0* head;
    UnkPendingNode_l3aa8e0* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void PushBack(UnkPendingNode_l3aa8e0* node) {
        if (count) {
            tail->next = node;
            node->next = nullptr;
            tail = node;
        } else {
            head = node;
            tail = node;
        }
        ++count;
    }
};

bool fn_71003aa8e0(UnkBehaviorTree_l3aa8e0* self, f32 dt) {
    UnkPendingNode_l3aa8e0* first = self->head;
    if (!first)
        return false;
    UnkBehaviorNode_A2_b69* node = first->key;
    u32 state = first->state;
    self->head = first->next;
    self->allocator->vf_0x48(first);
    if (--self->count == 0)
        self->tail = self->head;
    if (!node)
        return false;
    if (state - 1 <= 2)
        state = fn_7100394b30(node, dt, state);
    if (state - 5 <= 1) {
        if (UnkListenerHolder_l3aa8e0* holder = node->holder)
            if (holder->listener)
                holder->listener->vf_0x38(node, state);
    } else if (state == 7) {
        if (UnkListenerHolder_l3aa8e0* holder = node->holder)
            if (holder->listener)
                holder->listener->vf_0x40(node);
    } else {
        void* mem = self->allocator->vf_0x40(sizeof(UnkPendingNode_l3aa8e0), 8, self->flags);
        if (mem)
            self->PushBack(new (mem) UnkPendingNode_l3aa8e0(node, state));
    }
    return true;
}
