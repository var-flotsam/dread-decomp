// Sets the translation column of a node's 3x4 matrix: if any component differs from the new
// value by more than 0.001, stores all three, marks the node dirty and hands the new value to the
// embedded sphere (fn_7100216978).
#include "game/UnkMatrixNode.h"
#include "game/UnkSphere.h"

void fn_71002079a8(UnkMatrixNode* self, const UnkVec3f* v) {
    if (UNK_CHANGED(self->x_0x24, v->x) || UNK_CHANGED(self->y_0x34, v->y) ||
        UNK_CHANGED(self->z_0x44, v->z)) {
        const UnkVec3f p = *v;
        self->x_0x24 = p.x;
        self->y_0x34 = p.y;
        self->z_0x44 = p.z;
        self->dirty_0xfa = 1;
        fn_7100216978(&self->sphere_0x80, v);
    }
}
