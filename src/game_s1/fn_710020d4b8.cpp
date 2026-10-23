// Sets the translation of a transform node (include/game/UnkXformNode.h) when any component
// differs from the current one by more than 0.001: writes the matrix's translation column, marks
// the node dirty, passes the transform to the target (fn_71001c2480) and the new translation to
// the object at +0x80 (fn_7100216978).
#include "game/UnkXformNode.h"

void fn_71001c2480(UnkXformTarget* target, UnkXform* x);
void fn_7100216978(char* target, const UnkVec3f* translation);

void fn_710020d4b8(UnkXformNode* self, const UnkVec3f* v) {
    UnkXform* x = self->Xform();
    if (UNK_CHANGED(x->m[0][3], v->x) || UNK_CHANGED(x->m[1][3], v->y) || UNK_CHANGED(x->m[2][3], v->z)) {
        x->SetTranslation(*v);
        self->dirty_0xfa = 1;
        fn_71001c2480(self->target_0x108, x);
        fn_7100216978(reinterpret_cast<char*>(self) + 0x80, v);
    }
}
