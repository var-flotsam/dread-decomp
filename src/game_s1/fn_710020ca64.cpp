// Sets the translation column of a node's 3x4 matrix: if any component differs from the new
// value by more than 0.001, stores all three, marks the node dirty, notifies the embedded
// sub-object and forwards the new value to the target object.
#include "types.h"

struct UnkMatrixSub;
struct UnkVec3f {
    f32 x, y, z;
};
void fn_7100216978(UnkMatrixSub* s, const UnkVec3f* v);
struct UnkTarget;
void fn_71001bdd50(UnkTarget* t, const UnkVec3f* v);

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
    u8 pad_0xfb[0xd];
    UnkTarget* target_0x108;
};

#define UNK_CHANGED(a, b) ((a) - 0.001f > (b) || (a) + 0.001f < (b))

void fn_710020ca64(UnkMatrixNode* self, const UnkVec3f* v) {
    if (UNK_CHANGED(self->x_0x24, v->x) || UNK_CHANGED(self->y_0x34, v->y) ||
        UNK_CHANGED(self->z_0x44, v->z)) {
        self->z_0x44 = v->z;
        self->x_0x24 = v->x;
        self->y_0x34 = v->y;
        self->dirty_0xfa = 1;
        fn_7100216978((UnkMatrixSub*)self->sub_0x80, v);
        fn_71001bdd50(self->target_0x108, v);
    }
}
