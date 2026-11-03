// From unit 2739 of tools/tu_map.py (no reflected class): flushes the queue of items at +0xc8 (a
// counted singly linked list with its allocator), handing each item to fn_71011a91c4 without
// deferring, frees the nodes, clears the list and finishes with fn_71011a9980. Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkStruct_71011a91c4;
struct UnkItem_71011a91c4;
struct UnkModelManager;
void fn_71011a91c4(UnkModelManager* self, void* item, u32 flag);

struct UnkQueueOwner;
void fn_71011a9980(UnkQueueOwner* self);

struct UnkQueueNode {
    UnkItem_71011a91c4* item;
    UnkQueueNode* next;
};

struct UnkQueueOwner {
    u8 pad_0x0[0xc8];
    u32 count;
    UnkQueueNode* head;
    UnkQueueNode* tail;
    base::UnkAllocator* allocator;
};

void fn_71011aa820(UnkQueueOwner* self) {
    if (self->head) {
        for (UnkQueueNode* n = self->head; n; n = n->next)
            fn_71011a91c4(reinterpret_cast<UnkModelManager*>(self), n->item, 0);
        for (UnkQueueNode* n = self->head; n;) {
            UnkQueueNode* next = n->next;
            self->allocator->vf_0x48(n);
            n = next;
        }
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    fn_71011a9980(self);
}
