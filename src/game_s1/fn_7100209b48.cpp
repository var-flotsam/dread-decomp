// Sets the rotation angles of a transform node (include/game/UnkXformNode.h), but tests the new
// value against the translation column of the matrix, not the current angles: when any component
// differs by more than 0.001, caches the scale from the matrix if only the matrix is valid,
// stores the angles, clears the matrix-valid bit and marks the node dirty.
#include "game/UnkXformNode.h"

void fn_7100209b48(UnkXformNode* self, const UnkVec3f* v) {
    UnkXform* x = self->Xform();
    if (UNK_CHANGED(x->m[0][3], v->x) || UNK_CHANGED(x->m[1][3], v->y) || UNK_CHANGED(x->m[2][3], v->z)) {
        if ((self->flags_0x70 & 0xc) == 8) {
            self->scale_0x64 = fn_7100076cec(x);
            self->flags_0x70 |= 4;
        }
        self->angle_0x58 = *v;
        self->flags_0x70 = (self->flags_0x70 & ~8u) | 2;
        self->dirty_0xfa = 1;
    }
}
