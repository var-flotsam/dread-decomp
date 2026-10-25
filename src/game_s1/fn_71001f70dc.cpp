// Matrix setters of transform nodes that also pass the new translation on: when the matrix
// differs from the current one (read through fn_710002614c), copy it in, mark the matrix valid
// and clear the flags in a per-type global mask, mark the node dirty, then
// hand the translation to the sphere at +0x80 (fn_7100216978).
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"

extern u32 g_unk_716e597848 __attribute__((visibility("hidden")));
extern u32 g_unk_716e597ea8 __attribute__((visibility("hidden")));
extern u32 g_unk_716e598d20 __attribute__((visibility("hidden")));
extern u32 g_unk_716e598d38 __attribute__((visibility("hidden")));

// DirtyFirst: marks the node dirty before the copy, and compares in the other operand order.
template <bool DirtyFirst>
static inline void SetMatrixAndTranslation(UnkXformNode* self, const UnkMatrix44* m, const u32& mask) {
    UnkXform* x = self->Xform();
    const UnkMatrix44* cur = &x->GetMatrix();
    if ((DirtyFirst ? __builtin_memcmp(m, cur, sizeof(UnkMatrix44)) : __builtin_memcmp(cur, m, sizeof(UnkMatrix44))) == 0)
        return;
    if (DirtyFirst)
        self->dirty_0xfa = 1;
    for (int i = 0; i < 16; ++i)
        (&x->m[0][0])[i] = m->m[i];
    self->flags_0x70 = (self->flags_0x70 | 8) & ~mask;
    if (!DirtyFirst)
        self->dirty_0xfa = 1;
    UnkVec3f t = x->GetTranslation();
    fn_7100216978(UnkXformNodeSphere(self), &t);
}

void fn_71001f70dc(UnkXformNode* self, const UnkMatrix44* m) {
    SetMatrixAndTranslation<false>(self, m, g_unk_716e597848);
}

void fn_71001fb650(UnkXformNode* self, const UnkMatrix44* m) {
    SetMatrixAndTranslation<false>(self, m, g_unk_716e597ea8);
}

void fn_7100207d58(UnkXformNode* self, const UnkMatrix44* m) {
    SetMatrixAndTranslation<false>(self, m, g_unk_716e598d20);
}

void fn_7100209120(UnkXformNode* self, const UnkMatrix44* m) {
    SetMatrixAndTranslation<true>(self, m, g_unk_716e598d38);
}
