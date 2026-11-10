// Unit 127: the transform of one bone of the owner's skinned model: the bone's matrix (identity
// to start with, then fn_71001c1b70's) with the matrix-valid flag set and the global's bits cleared,
// or, for a negative index, the owner's own transform (+0x38) copied. Names are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkBoneOwner;
bool fn_71001c1b70(UnkBoneOwner* self, u32 index, UnkMatrix44* out);
extern u32 g_unk_710b3729d8 __attribute__((visibility("hidden")));

struct UnkBoneXformOwner_ovb0262h {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x98[0xa0];
    UnkBoneOwner* bones;  // 0x138
};
static_assert(__builtin_offsetof(UnkBoneXformOwner_ovb0262h, bones) == 0x138, "layout");

__attribute__((always_inline)) static inline void SetIdentity_ovb0262h(UnkMatrix44* m) {
    // Zeroes everything, then sets the diagonal from the last element.
    for (int i = 0; i < 16; ++i)
        m->m[i] = 0.0f;
    for (int i = 3; i >= 0; --i)
        m->m[i * 5] = 1.0f;
}

bool fn_71001437f8(UnkBoneXformOwner_ovb0262h* self, UnkXform* out, s32 index) {
    if (index >= 0) {
        UnkMatrix44 matrix;
        SetIdentity_ovb0262h(&matrix);
        if (!fn_71001c1b70(self->bones, index, &matrix))
            return false;
        UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(out->m), matrix);
        out->flags = (out->flags | 8) & ~g_unk_710b3729d8;
    } else if (&self->xform != out) {
        UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(out->m), *reinterpret_cast<const UnkMatrix44*>(self->xform.m));
        out->angle = self->xform.angle;
        out->scale = self->xform.scale;
        out->flags = self->xform.flags;
    }
    return true;
}
