// Removes the first node holding `value` from the singly linked list at +0xaa0 (count, head,
// tail, allocator), clearing head and tail when it becomes empty, and frees the node.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNode_7100872908 {
    void* value;
    UnkNode_7100872908* next;
};

struct UnkOwner_7100872908 {
    u8 pad_0x0[0xaa8];
    u32 count;                   // 0xaa8
    UnkNode_7100872908* head;    // 0xab0
    UnkNode_7100872908* tail;    // 0xab8
    base::UnkAllocator* allocator;  // 0xac0
};

void fn_7100872908(UnkOwner_7100872908* self, void* value) {
    UnkNode_7100872908* node = self->head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkNode_7100872908* prev = nullptr;
    UnkNode_7100872908* cur = self->head;
    UnkNode_7100872908* next;
    UnkNode_7100872908* last = nullptr;
    for (;;) {
        if (!cur)
            return;
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
}
