// Sets the matrix of a transform node (include/game/UnkXformNode.h) when it differs from the
// current one (read through fn_710002614c): copies it in, marks the matrix valid and clears the
// flags in g_unk_716e598dd8, marks the node dirty, passes the transform to the target
// (fn_71001c2480) and then the new translation to the sphere at +0x80 (fn_7100216978).
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"

extern u32 g_unk_716e598dd8 __attribute__((visibility("hidden")));

void fn_710020d744(UnkXformNode* self, const UnkMatrix44* m) {
    UnkXform* x = self->Xform();
    if (__builtin_memcmp(&x->GetMatrix(), m, sizeof(UnkMatrix44)) == 0)
        return;
    for (int i = 0; i < 16; ++i)
        (&x->m[0][0])[i] = m->m[i];
    self->flags_0x70 = (self->flags_0x70 | 8) & ~g_unk_716e598dd8;
    self->dirty_0xfa = 1;
    fn_71001c2480(self->target_0x108, x);
    UnkVec3f t = x->GetTranslation();
    fn_7100216978(UnkXformNodeSphere(self), &t);
}
