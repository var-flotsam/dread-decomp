// behaviortree::CBehaviorTree (unit 548): replaces the pending entry for `key` with a new one
// holding `state`, pushed at the front. False when there was no entry for the key.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkPendingNode_l3aac0c {
    virtual void vf_0x0();
    u32 state;
    void* key;
    UnkPendingNode_l3aac0c* next;

    UnkPendingNode_l3aac0c(void* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkBehaviorTree_l3aac0c {
    u8 pad_0x0[0x20];
    union {  // no type-based alias info: reloaded after the node's link is stored
        u32 count;
    };
    UnkPendingNode_l3aac0c* head;
    UnkPendingNode_l3aac0c* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void Remove(UnkPendingNode_l3aac0c* node) {
        UnkPendingNode_l3aac0c* prev = nullptr;
        for (UnkPendingNode_l3aac0c* cur = head; cur;) {
            UnkPendingNode_l3aac0c* next = cur->next;
            if (cur == node) {
                if (prev) {
                    prev->next = next;
                    if (tail == node)
                        tail = prev;
                } else {
                    head = next;
                }
                if (--count == 0) {
                    head = nullptr;
                    tail = nullptr;
                }
                allocator->vf_0x48(node);
                return;
            }
            prev = cur;
            cur = next;
        }
    }

    void PushFront(UnkPendingNode_l3aac0c* node) {
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

bool fn_71003aac0c(UnkBehaviorTree_l3aac0c* self, void* key, u32 state) {
    UnkPendingNode_l3aac0c* n = self->head;
    for (; n; n = n->next) {
        if (n->key == key)
            break;
    }
    if (!n)
        return false;
    self->Remove(n);
    void* mem = self->allocator->vf_0x40(sizeof(UnkPendingNode_l3aac0c), 8, self->flags);
    if (mem)
        self->PushFront(new (mem) UnkPendingNode_l3aac0c(key, state));
    return true;
}
