// game::logic::collision::COBoxShape2D: scaling the box's half extents (+0xc0, +0xc4) by a 2D
// factor, then the shape's update (vtable slot 0x80). Field names are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkScaledOBoxShape2D {
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
    u8 pad_0x8[0xb8];
    f32 field_0xc0;
    f32 field_0xc4;
};

void fn_7100d68734(UnkScaledOBoxShape2D* self, const CVector2D* scale) {
    self->field_0xc0 = scale->x * self->field_0xc0;
    self->field_0xc4 = scale->y * self->field_0xc4;
    self->vf_0x80();
}
