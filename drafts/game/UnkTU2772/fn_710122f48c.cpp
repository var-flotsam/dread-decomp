// nn::atk unit: moves an item's list node into a priority-ordered intrusive list (scanned from the back).
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkLink {
    UnkAtkLink* prev;
    UnkAtkLink* next;
};

struct UnkAtkQueued {
    u8 pad_0x000[0x204];
    s32 priority;
    u8 pad_0x208[0x288 - 0x208];
    UnkAtkLink link;
};

struct UnkAtkQueue {
    u8 pad_0x0[8];
    UnkAtkLink head;
};

static inline s32 PriorityOf(UnkAtkLink* link) {
    return reinterpret_cast<UnkAtkQueued*>(reinterpret_cast<char*>(link) - __builtin_offsetof(UnkAtkQueued, link))->priority;
}

void fn_710122f48c(UnkAtkQueue* queue, UnkAtkQueued* item) {
    UnkAtkLink* head = &queue->head;
    UnkAtkLink* node = &item->link;
    if (head != node) {
        UnkAtkLink* next = node->next;
        UnkAtkLink* self = next->prev;
        node->prev->next = next;
        next->prev = node->prev;
        self->next = node;
        node->prev = self;
    }
    UnkAtkLink* pos = head;
    UnkAtkLink* front = head->next;
    if (front != head) {
        while (pos != front && PriorityOf(pos->prev) > item->priority)
            pos = pos->prev;
    }
    UnkAtkLink* old = node->prev;
    UnkAtkLink* before = pos->prev;
    node->prev = before;
    old->next = pos;
    pos->prev->next = node;
    pos->prev = old;
}
