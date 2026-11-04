// Collection slots 0x48 and 0x50 of a reflected singly linked list of 8-byte values (base::UnkSList):
// copy-assignment, freeing this list's nodes and pushing a copy of each of other's values
// (fn_71002403f4; fn_71002404c4 is the same code).

#include "base/UnkSList.h"
#include "types.h"

struct UnkSListTag_71002403f4;
using UnkValueSList_b60 = base::UnkSList<UnkSListTag_71002403f4>;

static inline __attribute__((always_inline)) void AssignSList(UnkValueSList_b60* self,
                                                             const UnkValueSList_b60* other) {
    if (self == other)
        return;
    UnkValueSList_b60::Node* n = self->head;
    while (n) {
        UnkValueSList_b60::Node* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    for (UnkValueSList_b60::Node* it = other->head; it; it = it->next)
        self->PushBack(it->value);
}

UnkValueSList_b60* fn_71002403f4(UnkValueSList_b60* self, const UnkValueSList_b60* other) {
    AssignSList(self, other);
    return self;
}

UnkValueSList_b60* fn_71002404c4(UnkValueSList_b60* self, const UnkValueSList_b60* other) {
    AssignSList(self, other);
    return self;
}


// Slot 0x128 (fn_7100240054): erases the value at an index, unlinking and freeing its node.
void fn_7100240054(UnkValueSList_b60* self, const u32* it) {
    u32 i = *it;
    if (self->count <= i)
        return;
    UnkValueSList_b60::Node* cur = self->head;
    if (!cur)
        return;
    UnkValueSList_b60::Node* next = cur->next;
    UnkValueSList_b60::Node* prev = nullptr;
    for (u32 k = 0; k != i; ++k) {
        if (!next)
            return;
        prev = cur;
        cur = next;
        next = next->next;
    }
    if (prev) {
        prev->next = next;
        if (cur == self->tail)
            self->tail = prev;
    } else {
        self->head = next;
    }
    if (--self->count == 0) {
        self->head = nullptr;
        self->tail = nullptr;
    }
    self->allocator->vf_0x48(cur);
}
