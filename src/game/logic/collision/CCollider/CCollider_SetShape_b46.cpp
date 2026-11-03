// game::logic::collision::CCollider unit (tools/tu_map.py unit 2503): replaces the object held at
// +0x158: the old one is released (deleted through its vtable slot 0x10 when it is owned, +0x160)
// with the change notice (vtable slot 0x38) sent, then the new one and whether it is owned are
// stored and the notice is sent again. Names and layouts are placeholders.
#include "types.h"

struct UnkColliderShape_b46a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // deletes
};

struct UnkColliderShapeHolder_b46a3 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkColliderShape_b46a3* shape;  // 0x158
    bool owned;                     // 0x160

    void vf_0x38() { reinterpret_cast<void (*)(UnkColliderShapeHolder_b46a3*)>(vtable[0x38 / 8])(this); }
};

void fn_7100d36da4(UnkColliderShapeHolder_b46a3* self, UnkColliderShape_b46a3* shape, bool owned) {
    if (self->owned) {
        if (self->shape) {
            self->shape->vf_0x10();
            self->shape = nullptr;
            self->owned = false;
            self->vf_0x38();
        }
    } else if (self->shape) {
        self->shape = nullptr;
        self->vf_0x38();
    }
    self->shape = shape;
    self->owned = owned;
    self->vf_0x38();
}
