// Sets the translation column of a node's 3x4 matrix: if any component differs from the new
// value by more than 0.001, stores all three, marks the node dirty and notifies the embedded
// sub-object.
#include "types.h"

struct UnkMatrixSub;
void fn_7100216978(UnkMatrixSub* s);

struct UnkMatrixNode {
    u8 pad_0x0[0x24];
    f32 x_0x24;
    u8 pad_0x28[0xc];
    f32 y_0x34;
    u8 pad_0x38[0xc];
    f32 z_0x44;
    u8 pad_0x48[0x38];
    u8 sub_0x80[0x7a];
    u8 dirty_0xfa;
};

struct UnkVec3f {
    f32 x, y, z;
};

#define UNK_CHANGED(a, b) ((a) - 0.001f > (b) || (a) + 0.001f < (b))

void fn_71002079a8(UnkMatrixNode* self, const UnkVec3f* v) {
    if (UNK_CHANGED(self->x_0x24, v->x) || UNK_CHANGED(self->y_0x34, v->y) ||
        UNK_CHANGED(self->z_0x44, v->z)) {
        self->z_0x44 = v->z;
        self->x_0x24 = v->x;
        self->y_0x34 = v->y;
        self->dirty_0xfa = 1;
        fn_7100216978((UnkMatrixSub*)self->sub_0x80);
    }
}
