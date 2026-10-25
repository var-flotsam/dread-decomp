// Follows a transform (UnkXform, include/game/UnkXformNode.h): when its matrix differs from the
// copy kept at +0xa4, passes its translation to the sphere at +0x20 (fn_7100216978) and takes a
// new copy of the matrix.
#include "game/UnkSphere.h"
#include "game/UnkXformNode.h"

struct UnkXformFollower {
    u8 pad_0x0[0x20];
    UnkSphere sphere_0x20;
    u8 pad_0x70[0x34];
    UnkMatrix44 matrix;  // 0xa4
};

void fn_71001bfd5c(UnkXformFollower* self, UnkXform* x) {
    if (__builtin_memcmp(&x->GetMatrix(), &self->matrix, sizeof(UnkMatrix44)) == 0)
        return;
    UnkVec3f t = x->GetTranslation();
    fn_7100216978(&self->sphere_0x20, &t);
    UnkCopyMatrix(&self->matrix, x->GetMatrix());
}
