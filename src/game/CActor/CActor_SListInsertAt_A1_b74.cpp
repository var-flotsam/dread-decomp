// Unit 341 (CActor's, as src/game/CActor/CActor_SListAssign_b60.cpp): collection slot 0x118 of two reflected
// singly linked lists (base::UnkSList): insert a value at an index. Inside the list the value goes before the
// node at the index through that list's out-of-line InsertBefore (fn_7100240a00, fn_7100240b18, tools/gen_clones.py
// copies); at or past the end it is pushed back inline. The range test is written in each function: inside an
// inlined helper taking the index as an argument, clang swaps the compare's operands. Names are placeholders.
#include "base/UnkSList.h"
#include "types.h"

struct UnkClone_fn_7100240a00;
struct UnkClone_fn_7100240b18;
using UnkSListA_A1_b74 = base::UnkSList<UnkClone_fn_7100240a00>;
using UnkSListB_A1_b74 = base::UnkSList<UnkClone_fn_7100240b18>;

extern template void UnkSListA_A1_b74::InsertBefore(const u64&, UnkSListA_A1_b74::Node*);
extern template void UnkSListB_A1_b74::InsertBefore(const u64&, UnkSListB_A1_b74::Node*);

template <class List>
static inline __attribute__((always_inline)) typename List::Node* At_A1_b74(List* self, u32 i) {
    typename List::Node* n = self->head;
    for (u32 k = 0; k < self->count; ++k, n = n->next)
        if (k == i)
            return n;
    return nullptr;
}

void fn_710023fedc(UnkSListA_A1_b74* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
        self->PushBack(*v);
        return;
    }
    self->InsertBefore(*v, At_A1_b74(self, i));
}

void fn_710023ff98(UnkSListB_A1_b74* self, const u32* it, const u64* v) {
    u32 i = *it;
    if (i >= self->count) {
        self->PushBack(*v);
        return;
    }
    self->InsertBefore(*v, At_A1_b74(self, i));
}
