// Removes the entry for `key` from a linked list of pending entries. Unless the entry's state
// was 1, a new entry for the key with state 3 is pushed at the front. Returns true only when
// the removed entry had state 1; false when there was no entry or a new one was queued.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkNode_71003aad48 {
    virtual void vf_0x0();
    u32 state;
    void* key;
    UnkNode_71003aad48* next;

    UnkNode_71003aad48(void* k, u32 s) : state(s), key(k), next(nullptr) {}
};

struct UnkStruct_71003aad48 {
    u8 pad_0x0[0x20];
    union {  // no type-based alias info: reloaded after the node's link is stored
        u32 count;
    };
    UnkNode_71003aad48* head;
    UnkNode_71003aad48* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    void Remove(UnkNode_71003aad48* node) {
        UnkNode_71003aad48* prev = nullptr;
        for (UnkNode_71003aad48* cur = head; cur;) {
            UnkNode_71003aad48* next = cur->next;
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

    void PushFront(UnkNode_71003aad48* node) {
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

// NON_MATCHING(minor): when pushing onto a non-empty list, the count increment is scheduled
// after the head store instead of before it.
bool fn_71003aad48(UnkStruct_71003aad48* self, void* key) {
    UnkNode_71003aad48* n = self->head;
    for (; n; n = n->next) {
        if (n->key == key)
            break;
    }
    if (!n)
        return false;
    u32 state = n->state;
    self->Remove(n);
    if (state == 1)
        return true;
    void* mem = self->allocator->vf_0x40(sizeof(UnkNode_71003aad48), 8, self->flags);
    if (!mem)
        return false;
    self->PushFront(new (mem) UnkNode_71003aad48(key, 3));
    return false;
}
