// Unit 274 (tools/tu_map.py, no reliable name): setters of a transform node (include/game/UnkXformNode.h)
// that pass the changed transform on to the target's matrix copy (the target's holder at +0x78, its
// matrix at +0x28): a whole matrix, and the angles. Names and layouts are placeholders.
#include "game/UnkXformNode.h"

extern u32 g_unk_716e597830 __attribute__((visibility("hidden")));

struct UnkMatrixHolder_b0261 {
    u8 pad_0x0[0x28];
    UnkMatrix44 matrix;  // 0x28
};

struct UnkMatrixTarget_b0261 {
    u8 pad_0x0[0x78];
    UnkMatrixHolder_b0261* holder;  // 0x78
};

void fn_71001f4a60(UnkXformNode* self, const UnkMatrix44* m) {
    UnkXform* x = self->Xform();
    const UnkMatrix44* cur = &x->GetMatrix();
    if (__builtin_memcmp(cur, m, sizeof(UnkMatrix44)) == 0)
        return;
    for (int i = 0; i < 16; ++i)
        (&x->m[0][0])[i] = m->m[i];
    self->flags_0x70 = (self->flags_0x70 | 8) & ~g_unk_716e597830;
    UnkMatrixTarget_b0261* target = reinterpret_cast<UnkMatrixTarget_b0261*>(self->target_0x108);
    const UnkMatrix44& n = x->GetMatrix();
    UnkCopyMatrix(&target->holder->matrix, n);
}

struct UnkXformModel_b0261 {
    u8 pad_0x0[0x28];
    UnkMatrix44 matrix;
};

void fn_71001f45a4(UnkXformNode* self, const UnkVec3f* v) {
    UnkXform* x = self->Xform();
    UnkVec3f angle = self->GetAngle(x);
    if (UNK_CHANGED(angle.x, v->x) || UNK_CHANGED(angle.y, v->y) || UNK_CHANGED(angle.z, v->z)) {
        x->SetAngle(*v);
        UnkXformTarget* target = self->target_0x108;
        const UnkMatrix44& m = x->GetMatrix();
        UnkCopyMatrix(&static_cast<UnkXformModel_b0261*>(target->field_0x78)->matrix, m);
    }
}
