// Sets the matrix of a transform node (include/game/UnkXformNode.h) when it differs from the
// current one (read through fn_710002614c): copies it in, marks the matrix valid and clears the
// flags in g_unk_716e598cf0, marks the node dirty, then hands the position from the node's
// virtual at 0x98 to the sphere at +0x80 (fn_7100216978).
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"

extern u32 g_unk_716e598cf0 __attribute__((visibility("hidden")));

struct UnkXformNodeVtbl {
    u8 pad_0x0[0x98];
    UnkVec3f (*vf_0x98)(UnkXformNode* self);
};

void fn_7100206294(UnkXformNode* self, const UnkMatrix44* m) {
    UnkXform* x = self->Xform();
    if (__builtin_memcmp(&x->GetMatrix(), m, sizeof(UnkMatrix44)) == 0)
        return;
    for (int i = 0; i < 16; ++i)
        (&x->m[0][0])[i] = m->m[i];
    self->flags_0x70 = (self->flags_0x70 | 8) & ~g_unk_716e598cf0;
    self->dirty_0xfa = 1;
    UnkSphere* sphere = UnkXformNodeSphere(self);
    UnkVec3f pos = (*reinterpret_cast<UnkXformNodeVtbl**>(self))->vf_0x98(self);
    fn_7100216978(sphere, &pos);
}
