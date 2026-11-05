// Copies of fn_7100240294 (src/game/CActor/CActor_SListSetAt_A1_b74.cpp) in CEntity's unit: collection slot 0x140
// of two reflected singly linked lists (base::UnkSList), replacing the value at an index. The node there is
// unlinked and freed; the value then goes before the node that followed it through that list's out-of-line
// InsertBefore (fn_7100d83598, fn_7100d836b0, tools/gen_clones.py copies), or is pushed back inline when there
// was none or the index is out of range. Names are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkClone_fn_7100d83598;
struct UnkClone_fn_7100d836b0;
using UnkSListA_A6_b76 = base::UnkSList<UnkClone_fn_7100d83598>;
using UnkSListB_A6_b76 = base::UnkSList<UnkClone_fn_7100d836b0>;

extern template void UnkSListA_A6_b76::InsertBefore(const u64&, UnkSListA_A6_b76::Node*);
extern template void UnkSListB_A6_b76::InsertBefore(const u64&, UnkSListB_A6_b76::Node*);

template <class List>
static inline __attribute__((always_inline)) void SetAt_A6_b76(List* self, const u32* it, const u64* v) {
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

void fn_7100d8316c(UnkSListA_A6_b76* self, const u32* it, const u64* v) {
    SetAt_A6_b76(self, it, v);
}

void fn_7100d83298(UnkSListB_A6_b76* self, const u32* it, const u64* v) {
    SetAt_A6_b76(self, it, v);
}
