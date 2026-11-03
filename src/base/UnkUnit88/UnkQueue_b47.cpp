// Unit 88 (tools/tu_map.py): adds a value at the front of (fn_71000ea374) and removes the first node of a singly linked queue (count +0x150, head +0x158,
// tail +0x160), freeing it through the allocator at +0x168; the tail follows the head when the queue
// empties. Names are placeholders.
#include "base/UnkVector.h"

struct UnkQueueNode_b47 {
    void* value;
    UnkQueueNode_b47* next;  // 0x8
};

struct UnkQueueOwner_b47 {
    u8 pad_0x0[0x150];
    u32 count;                    // 0x150
    UnkQueueNode_b47* head;       // 0x158
    UnkQueueNode_b47* tail;       // 0x160
    base::UnkAllocator* allocator;  // 0x168
    u32 flags;                      // 0x170
};

void fn_71000ea3e8(UnkQueueOwner_b47* self) {
    UnkQueueNode_b47* node = self->head;
    if (!node)
        return;
    self->head = node->next;
    self->allocator->vf_0x48(node);
    if (--self->count == 0)
        self->tail = self->head;
}

void fn_71000ea374(UnkQueueOwner_b47* self, void* value) {
    if (!value)
        return;
    UnkQueueNode_b47* node = static_cast<UnkQueueNode_b47*>(self->allocator->vf_0x40(0x10, 8, self->flags));
    if (!node)
        return;
    node->value = value;
    node->next = nullptr;
    if (self->count == 0) {
        self->head = node;
        self->tail = node;
        self->count = 1;
    } else {
        // Copied bytewise: the original reloads the count after this store.
        __builtin_memcpy(&node->next, &self->head, sizeof(node->next));
        self->head = node;
        ++self->count;
    }
}
