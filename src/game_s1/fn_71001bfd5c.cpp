// Follows a transform (UnkXform, include/game/UnkXformNode.h): when its matrix differs from the
// copy kept at +0xa4, passes its translation to the object at +0x20 (fn_7100216978) and takes a
// new copy of the matrix.
#include "game/UnkXformNode.h"

struct UnkXformFollower {
    u8 pad_0x0[0x20];
    char target_0x20[0x84];
    UnkMatrix44 matrix;  // 0xa4
};

void fn_7100216978(char* target, const UnkVec3f* translation);

void fn_71001bfd5c(UnkXformFollower* self, UnkXform* x) {
    if (__builtin_memcmp(&x->GetMatrix(), &self->matrix, sizeof(UnkMatrix44)) == 0)
        return;
    UnkVec3f t = x->GetTranslation();
    fn_7100216978(self->target_0x20, &t);
    UnkCopyMatrix(&self->matrix, x->GetMatrix());
}
