// CActor's unit (tools/tu_map.py unit 341): collection slot 0x128 (fn_7100248338) of a reflected chained
// hash map keyed by an object pointer (hashed by the two words at +0x30 of the object): the key's node
// is unlinked from its bucket chain (+0x40) and from the list of all nodes (prev +0x30, next +0x38; the
// map's ends at +0x40 and +0x38), then released through fn_71002486a8, and the count drops.
// Names and layouts are placeholders.
#include "types.h"

struct UnkNodeMap_710025ad0c;
struct UnkNode_710025ad0c;
void fn_71002486a8(UnkNodeMap_710025ad0c* map, UnkNode_710025ad0c* node);  // removes a node

struct UnkHashedObject_b61 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkPtrNode_b61 {
    UnkHashedObject_b61* key;
    u8 pad_0x8[0x28];
    UnkPtrNode_b61* prev;   // 0x30
    UnkPtrNode_b61* next;   // 0x38
    UnkPtrNode_b61* chain;  // 0x40

    // The list links, through accessors (their stores may alias each other).
    UnkPtrNode_b61*& Next() { return next; }
    UnkPtrNode_b61*& Prev() { return prev; }
};

struct UnkPtrMap_b61 {
    u8 pad_0x0[0xc];
    u32 count;                 // 0xc
    UnkPtrNode_b61** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkPtrNode_b61* tail;  // 0x38
    UnkPtrNode_b61* head;  // 0x40
};

void fn_7100248338(UnkPtrMap_b61* self, UnkHashedObject_b61* const* key) {
    UnkHashedObject_b61* k = *key;
    u32 b = (k->field_0x30 ^ k->field_0x34) % self->bucket_count;
    UnkPtrNode_b61** slot = self->buckets + b;
    UnkPtrNode_b61* n = *slot;
    if (!n)
        return;
    if (n->key != k) {
        UnkPtrNode_b61* before;
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
    fn_71002486a8(reinterpret_cast<UnkNodeMap_710025ad0c*>(self), reinterpret_cast<UnkNode_710025ad0c*>(n));
    --self->count;
}
