// Unit 341 (CActor's, as src/game/CActor/CActor_SListAssign_b60.cpp): collection slot 0x140 of two reflected
// singly linked lists (base::UnkSList): replace the value at an index. The node there is unlinked and freed as in
// the list's erase (src/game/CEntity/CEntity_SListErase_A6_b63.cpp); the value then goes before the node that
// followed it, through that list's out-of-line InsertBefore (fn_7100240a00, fn_7100240b18, tools/gen_clones.py
// copies), or is pushed back inline when there was none or the index is out of range. The push-back block comes
// first in the source, as in the original's block order. Names are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkClone_fn_7100240a00;
struct UnkClone_fn_7100240b18;
using UnkSListA_A1_b74 = base::UnkSList<UnkClone_fn_7100240a00>;
using UnkSListB_A1_b74 = base::UnkSList<UnkClone_fn_7100240b18>;

extern template void UnkSListA_A1_b74::InsertBefore(const u64&, UnkSListA_A1_b74::Node*);
extern template void UnkSListB_A1_b74::InsertBefore(const u64&, UnkSListB_A1_b74::Node*);

template <class List>
static inline __attribute__((always_inline)) void SetAt_A1_b74(List* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
    push:
        self->PushBack(*v);
        return;
    }
    {
        typename List::Node* cur = self->head;
        if (!cur)
            goto push;
        typename List::Node* next = cur->next;
        typename List::Node* prev = nullptr;
        for (u32 k = 0; k != i; ++k) {
            if (!next)
                goto push;
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
        if (next) {
            self->InsertBefore(*v, next);
            return;
        }
    }
    goto push;
}

void fn_7100240168(UnkSListA_A1_b74* self, const u32* it, const u64* v) {
    SetAt_A1_b74(self, it, v);
}

void fn_7100240294(UnkSListB_A1_b74* self, const u32* it, const u64* v) {
    SetAt_A1_b74(self, it, v);
}
