// Unit 2702 (hint CAIComponent): resets an AI component's state block to its defaults.
// Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "types.h"

struct UnkResetTarget_l0fa790 {
    u8 pad_0x0[0x36a];
    bool field_0x36a;
};

struct UnkResetState_l0fa790 {
    u16 field_0x0;
    u8 pad_0x2[2];
    f32 field_0x4;
    f32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 pad_0x14[4];
    base::math::CVector3D field_0x18;
    base::math::CVector3D field_0x24;
    u8 field_0x30;
    bool field_0x31;
    u16 field_0x32;
    u8 field_0x34;
    u8 pad_0x35[3];
};

struct UnkResetSelf_l0fa790 {
    u8 pad_0x0[0x11e4];
    u32 field_0x11e4;
    u32 field_0x11e8;
    UnkResetState_l0fa790 state;  // 0x11ec
    u8 pad_0x1224[0x2d8];
    f32 field_0x14fc;
    u8 pad_0x1500[0x110];
    UnkResetTarget_l0fa790* field_0x1610;
};

void fn_71010fa790(UnkResetSelf_l0fa790* self) {
    UnkResetState_l0fa790* s = &self->state;
    s->field_0x32 = 0;
    s->field_0x34 = 0;
    self->field_0x14fc = 9999.9f;
    self->field_0x11e4 = 1;
    self->field_0x11e8 = 15;
    s->field_0x4 = -1.0f;
    s->field_0x8 = 0.0f;
    s->field_0xc = 0;
    s->field_0x18.x = g_unk_710164779c.x;
    s->field_0x18.y = g_unk_710164779c.y;
    s->field_0x18.z = g_unk_710164779c.z;
    s->field_0x24.x = g_unk_710164779c.x;
    s->field_0x24.y = g_unk_710164779c.y;
    s->field_0x24.z = g_unk_710164779c.z;
    s->field_0x0 = 0;
    s->field_0x10 = 0;
    bool value = false;
    if (s->field_0x30 == 0)
        value = !self->field_0x1610->field_0x36a;
    s->field_0x31 = value;
}
