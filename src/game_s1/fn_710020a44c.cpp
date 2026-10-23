// Pushes a transform node's pending changes to its target (+0x108) when the node is active
// (+0xf9): a dirty matrix goes out as the product of the node's matrix (fn_710002614c) and the
// one at +0x110 (fn_71000127c8) through fn_710019c474; a pending flag at +0xfb calls
// fn_71001c34f4 with 0. Returns whether the node is active.
#include "game/UnkXformNode.h"

UnkMatrix44 fn_71000127c8(const UnkMatrix44& a, const UnkMatrix44& b);  // a * b
void fn_710019c474(void* target, const UnkMatrix44* m);
void fn_71001c34f4(void* target, f32 value);

struct UnkXformNodeWithOffset : UnkXformNode {
    UnkMatrix44 matrix_0x110;  // applied after the node's own matrix
};

bool fn_710020a44c(UnkXformNodeWithOffset* self) {
    if (self->active_0xf9) {
        if (self->dirty_0xfa) {
            UnkXformTarget* target = self->target_0x108;
            self->dirty_0xfa = 0;
            UnkMatrix44 m = fn_71000127c8(self->Xform()->GetMatrix(), self->matrix_0x110);
            fn_710019c474(target->field_0x78, &m);
        }
        if (self->flag_0xfb) {
            fn_71001c34f4(self->target_0x108, 0.0f);
            self->flag_0xfb = 0;
        }
    }
    return self->active_0xf9;
}
