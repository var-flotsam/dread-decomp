// Sets the rotation angles of a transform node (include/game/UnkXformNode.h) when any component
// differs from the current angles by more than 0.001: first caches the scale from the matrix if
// only the matrix is valid, then stores the angles, clears the matrix-valid bit, marks the node
// dirty and passes the transform on to the target (fn_71001c2480).
#include "game/UnkXformNode.h"

void fn_71001c2480(UnkXformTarget* target, UnkXform* x);

void fn_710020d5b8(UnkXformNode* self, const UnkVec3f* v) {
    UnkVec3f cur = self->GetAngle();
    if (UNK_CHANGED(cur.x, v->x) || UNK_CHANGED(cur.y, v->y) || UNK_CHANGED(cur.z, v->z)) {
        UnkXform* x = self->Xform();
        if ((self->flags_0x70 & 0xc) == 8) {
            self->scale_0x64 = fn_7100076cec(x);
            self->flags_0x70 |= 4;
        }
        self->angle_0x58 = *v;
        self->flags_0x70 = (self->flags_0x70 & ~8u) | 2;
        self->dirty_0xfa = 1;
        fn_71001c2480(self->target_0x108, x);
    }
}
