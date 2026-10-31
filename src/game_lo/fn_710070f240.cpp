// Removes the first node holding `value` from the singly linked list at +0x8 (count, head,
// tail, allocator), clearing head and tail when it becomes empty, and frees the node; the same
// shape as fn_7100872908 for another owner.
#include "base/UnkVector.h"
#include "types.h"

struct UnkNode_710070f240 {
    u32 value;
    UnkNode_710070f240* next;
};

struct UnkOwner_710070f240 {
    u64 field_0x0;
    u32 count;
    UnkNode_710070f240* head;
    UnkNode_710070f240* tail;
    base::UnkAllocator* allocator;
};

void fn_710070f240(UnkOwner_710070f240* self, u32 value) {
    UnkNode_710070f240* node = self->head;
    for (; node; node = node->next) {
        if (node->value == value)
            break;
    }
    if (!node)
        return;
    UnkNode_710070f240* prev = nullptr;
    UnkNode_710070f240* cur = self->head;
    UnkNode_710070f240* next;
    UnkNode_710070f240* last = nullptr;
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
