// CEntity unit: copy of fn_7100248338 (src/game/CActor/CActor_PtrMapErase_b61.cpp), releasing through fn_7100d89f0c.
// Names and layouts are placeholders.
#include "types.h"

struct UnkNodeMap_7100d89b9c;
struct UnkNode_7100d89b9c;
void fn_7100d89f0c(UnkNodeMap_7100d89b9c* map, UnkNode_7100d89b9c* node);  // removes a node

struct UnkHashedObject_A6_b63 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkPtrNode_A6_b63 {
    UnkHashedObject_A6_b63* key;
    u8 pad_0x8[0x28];
    UnkPtrNode_A6_b63* prev;   // 0x30
    UnkPtrNode_A6_b63* next;   // 0x38
    UnkPtrNode_A6_b63* chain;  // 0x40

    // The list links, through accessors (their stores may alias each other).
    UnkPtrNode_A6_b63*& Next() { return next; }
    UnkPtrNode_A6_b63*& Prev() { return prev; }
};

struct UnkPtrMap_A6_b63 {
    u8 pad_0x0[0xc];
    u32 count;                 // 0xc
    UnkPtrNode_A6_b63** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkPtrNode_A6_b63* tail;  // 0x38
    UnkPtrNode_A6_b63* head;  // 0x40
};

void fn_7100d89b9c(UnkPtrMap_A6_b63* self, UnkHashedObject_A6_b63* const* key) {
    UnkHashedObject_A6_b63* k = *key;
    u32 b = (k->field_0x30 ^ k->field_0x34) % self->bucket_count;
    UnkPtrNode_A6_b63** slot = self->buckets + b;
    UnkPtrNode_A6_b63* n = *slot;
    if (!n)
        return;
    if (n->key != k) {
        UnkPtrNode_A6_b63* before;
        do {
            before = n;
            n = n->chain;
            if (!n)
                return;
        } while (n->key != k);
        slot = &before->chain;
    }
    *slot = n->chain;
    if (self->head == n)
        self->head = n->Prev();
    if (self->tail == n)
        self->tail = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    fn_7100d89f0c(reinterpret_cast<UnkNodeMap_7100d89b9c*>(self), reinterpret_cast<UnkNode_7100d89b9c*>(n));
    --self->count;
}
