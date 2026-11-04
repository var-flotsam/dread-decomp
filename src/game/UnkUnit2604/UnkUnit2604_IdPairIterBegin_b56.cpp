// Begin and end of the lists of UnkUnit2604_IdPairIter_b56.h: a temporary iterator from the list head
// (or null) is synced out of line (in another file, so the temporary escapes), assigned to the output,
// the output synced, and the temporary reset. Names and layouts are placeholders.
#include "UnkUnit2604_IdPairIter_b56.h"

struct UnkIdPairListOwner_b56 {
    u8 unk_0x0[0x40];
    u8* head;
};

__attribute__((always_inline)) inline void UnkIdPairListAt_b56(u8* node, UnkIdPairListIter_b56<0>* out) {
    UnkIdPairListIter_b56<0> it;
    it.node = node;
    it.state = 0;
    fn_7100f8fea0(&it);
    out->node = it.node;
    fn_7100f8fea0(out);
    it.Reset();
}

void fn_7100f923d0(UnkIdPairListOwner_b56* self, UnkIdPairListIter_b56<0>* out) {
    UnkIdPairListAt_b56(self->head, out);
}

void fn_7100f924c4(UnkIdPairListOwner_b56* self, UnkIdPairListIter_b56<0>* out) {
    UnkIdPairListAt_b56(nullptr, out);
}
