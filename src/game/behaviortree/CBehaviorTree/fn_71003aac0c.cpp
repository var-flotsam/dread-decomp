// behaviortree::CBehaviorTree (unit 548): replaces the pending entry for `key` with a new one
// holding `state`, pushed at the front. False when there was no entry for the key.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkPendingNode_l3aac0c {
    virtual void vf_0x0();
    u32 state;
    void* key;
    UnkPendingNode_l3aac0c* next;

    UnkPendingNode_l3aac0c(void* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkInverterTarget_b66a2 {
    u8 pad_0x0[0x20];
    base::UnkLinkedList<UnkPendingNode_l3aac0c> pending;  // 0x20
};

bool fn_71003aac0c(UnkInverterTarget_b66a2* self, UnkInverter_b66a2* key, s32 state) {
    UnkPendingNode_l3aac0c* n = self->pending.head;
    for (; n; n = n->next) {
        if (n->key == key)
            break;
    }
    if (!n)
        return false;
    self->pending.Remove(n);
    void* mem = self->pending.allocator->vf_0x40(sizeof(UnkPendingNode_l3aac0c), 8, self->pending.flags);
    if (mem)
        self->pending.PushFront(new (mem) UnkPendingNode_l3aac0c(key, state));
    return true;
}
