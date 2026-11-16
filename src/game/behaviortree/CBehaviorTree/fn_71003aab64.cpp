// behaviortree::CBehaviorTree (unit 548): pushes a pending entry with state 1 for `key` at the
// front of the list of pending entries; false for a null key. Names and layouts are placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkPendingNode_l3aab64 {
    virtual void vf_0x0();
    u32 state;
    void* key;
    UnkPendingNode_l3aab64* next;

    UnkPendingNode_l3aab64(void* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkSelectorTree_A2_b68 {
    u8 pad_0x0[0x20];
    base::UnkLinkedList<UnkPendingNode_l3aab64> pending;  // 0x20
};

bool fn_71003aab64(UnkSelectorTree_A2_b68* self, UnkSelectorNode_A2_b68* key) {
    if (!key)
        return false;
    void* mem = self->pending.allocator->vf_0x40(sizeof(UnkPendingNode_l3aab64), 8, self->pending.flags);
    if (mem)
        self->pending.PushFront(new (mem) UnkPendingNode_l3aab64(key, 1));
    return true;
}
