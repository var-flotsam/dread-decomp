// Begin and end of the lists of CCollider_RefIter_b56.h: a temporary iterator from the list head (or
// null) is synced out of line (in another file, so the temporary escapes), assigned to the output, the
// output synced, and the temporary reset. Names and layouts are placeholders.
#include "CCollider_RefIter_b56.h"

struct UnkRefListOwner_b56 {
    u8 unk_0x0[0x40];
    u8* head;
};

void fn_7100d41dfc(UnkRefListOwner_b56* self, UnkRefListIter_b56<0>* out) {
    UnkRefListIter_b56<0> it;
    it.node = self->head;
    it.variant.state = 0;
    fn_7100d3d538(&it);
    out->node = it.node;
    fn_7100d3d538(out);
    it.variant.Reset();
}

void fn_7100d41ecc(UnkRefListOwner_b56* self, UnkRefListIter_b56<0>* out) {
    UnkRefListIter_b56<0> it;
    it.node = nullptr;
    it.variant.state = 0;
    fn_7100d3d538(&it);
    out->node = it.node;
    fn_7100d3d538(out);
    it.variant.Reset();
}
