// Sets the angles of a transform node whose angles and scale are cached lazily: reads the cached
// angles (deriving them from the matrix if they are not valid yet), and when any component differs
// from the new value by more than 0.001, first caches the scale from the matrix if only the matrix
// is valid, then stores the new angles, clears the matrix-valid bit and marks the node dirty.
#include "game/UnkXformNode.h"

void fn_71001f2ddc(UnkXformNode* self, const UnkVec3f* v) {
    UnkVec3f cur = self->GetAngle();
    if (UNK_CHANGED(cur.x, v->x) || UNK_CHANGED(cur.y, v->y) || UNK_CHANGED(cur.z, v->z)) {
        if ((self->flags_0x70 & 0xc) == 8) {
            self->scale_0x64 = fn_7100076cec((UnkXform*)self->xform_0x18);
            self->flags_0x70 |= 4;
        }
        self->angle_0x58 = *v;
        self->flags_0x70 = (self->flags_0x70 & ~8u) | 2;
        self->dirty_0xfa = 1;
    }
}
