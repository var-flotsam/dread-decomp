// Sets the scale of a transform node (include/game/UnkXformNode.h) when any component differs
// from the current scale by more than 0.001: marks the node dirty, caches the angles from the
// matrix if only the matrix is valid, stores the scale, clears the matrix-valid bit and passes a
// copy of the new scale to the target (fn_71001c1f9c).
#include "game/UnkXformNode.h"

void fn_71001c1f9c(UnkXformTarget* target, const UnkVec3f* scale);

void fn_710020d33c(UnkXformNode* self, const UnkVec3f* v) {
    UnkVec3f cur = self->GetScale();
    if (UNK_CHANGED(cur.x, v->x) || UNK_CHANGED(cur.y, v->y) || UNK_CHANGED(cur.z, v->z)) {
        self->dirty_0xfa = 1;
        if ((self->flags_0x70 & 0xa) == 8) {
            self->angle_0x58 = fn_7100079b18((UnkXform*)self->xform_0x18, self->angle_0x58.z);
            self->flags_0x70 |= 2;
        }
        self->scale_0x64 = *v;
        UnkXformTarget* target = self->target_0x108;
        self->flags_0x70 = (self->flags_0x70 & ~0xcu) | 4;
        UnkVec3f scale = self->scale_0x64;
        fn_71001c1f9c(target, &scale);
    }
}
