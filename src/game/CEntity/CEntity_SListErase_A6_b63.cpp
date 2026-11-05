// CEntity unit: collection slot 0x128 of a reflected singly linked list (base::UnkSList), the same code as
// fn_7100240054 (src/game/CActor/CActor_SListAssign_b60.cpp): erase the value at an index, unlinking and
// freeing its node. Names are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkSListTag_7100d83058;
using UnkValueSList_A6_b63 = base::UnkSList<UnkSListTag_7100d83058>;

void fn_7100d83058(UnkValueSList_A6_b63* self, const u32* it) {
    u32 i = *it;
    if (self->count <= i)
        return;
    UnkValueSList_A6_b63::Node* cur = self->head;
    if (!cur)
        return;
    UnkValueSList_A6_b63::Node* next = cur->next;
    UnkValueSList_A6_b63::Node* prev = nullptr;
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
