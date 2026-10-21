// Sets the scale of a transform node whose scale and angles are cached lazily: reads the cached
// scale (deriving it from the matrix if it is not valid yet), and when any component differs from
// the new value by more than 0.001, first caches the angles from the matrix if only the matrix is
// valid, then stores the new scale, clears the matrix-valid bit and marks the node dirty.
#include "game/UnkXformNode.h"

void fn_71001fb4d4(UnkXformNode* self, const UnkVec3f* v) {
    UnkXform* xf = (UnkXform*)self->xform_0x18;
    UnkVec3f cur = self->GetScale(xf);
    if (UNK_CHANGED(cur.x, v->x) || UNK_CHANGED(cur.y, v->y) || UNK_CHANGED(cur.z, v->z)) {
        if ((self->flags_0x70 & 0xa) == 8) {
            self->angle_0x58 = fn_7100079b18(xf, self->angle_0x58.z);
            self->flags_0x70 |= 2;
        }
        self->scale_0x64 = *v;
        self->flags_0x70 = (self->flags_0x70 & ~0xcu) | 4;
        self->dirty_0xfa = 1;
    }
}
