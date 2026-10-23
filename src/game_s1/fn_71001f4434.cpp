// Sets the translation of a transform node (include/game/UnkXformNode.h) when any component
// differs from the current one by more than 0.001, passes it to the object at +0x80
// (fn_7100216978) and copies the node's matrix into the target's model (+0x78, matrix at +0x28).
#include "game/UnkXformNode.h"

struct UnkXformModel {
    u8 pad_0x0[0x28];
    UnkMatrix44 matrix;
};

void fn_7100216978(char* target, const UnkVec3f* translation);

void fn_71001f4434(UnkXformNode* self, const UnkVec3f* v) {
    UnkXform* x = self->Xform();
    if (UNK_CHANGED(x->m[0][3], v->x) || UNK_CHANGED(x->m[1][3], v->y) || UNK_CHANGED(x->m[2][3], v->z)) {
        x->SetTranslation(*v);
        fn_7100216978(reinterpret_cast<char*>(self) + 0x80, v);
        UnkXformTarget* target = self->target_0x108;
        const UnkMatrix44& m = x->GetMatrix();
        UnkCopyMatrix(&static_cast<UnkXformModel*>(target->field_0x78)->matrix, m);
    }
}
