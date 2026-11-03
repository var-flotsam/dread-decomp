// game::logic::collision::CCapsuleShape2D: scaling the shape by a 2D factor, the field at +0xb4 by
// its x and the one at +0xb0 by its y, then the shape's update (vtable slot 0x80). Field names are
// placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkScaledCapsuleShape2D {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    u8 pad_0x8[0xa8];
    f32 field_0xb0;
    f32 field_0xb4;
};

void fn_7100d601a0(UnkScaledCapsuleShape2D* self, const CVector2D* scale) {
    self->field_0xb4 = scale->x * self->field_0xb4;
    self->field_0xb0 = scale->y * self->field_0xb0;
    self->vf_0x80();
}
