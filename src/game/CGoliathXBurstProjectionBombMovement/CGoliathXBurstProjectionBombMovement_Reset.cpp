#include "types.h"
#include "base/math/UnkZeroVector3.h"

struct UnkUnit840Data {
    u8 unk_0x0[0x100];
    u32 field_0x100;
};

struct CGoliathXBurstProjectionBombMovement {
    u8 unk_0x0[0x4c0];
    u32 field_0x4c0;
    u8 unk_0x4c4[0x1c];
    UnkUnit840Data* data;
    u8 unk_0x4e8[0x8];
    u32 field_0x4f0;
    u16 field_0x4f4;
    u32 field_0x4f8;
    bool field_0x4fc;
    u32 field_0x500;
    u32 field_0x504;
    u32 field_0x508;
    u8 unk_0x50c[0xd28 - 0x50c];
    u32 field_0xd28;
    u8 unk_0xd2c[0xd70 - 0xd2c];
    u64 field_0xd70;
    u64 field_0xd78;
    u64 field_0xd80;
    u8 unk_0xd88[0xd9c - 0xd88];
    base::math::CVector3D field_0xd9c;
    u16 field_0xda8;
};

void fn_7100aaa024(CGoliathXBurstProjectionBombMovement* self);

void fn_710058a668(CGoliathXBurstProjectionBombMovement* self) {
    fn_7100aaa024(self);
    self->field_0x4f0 = self->data->field_0x100;
    self->field_0x4f4 = 0;
    self->field_0x4c0 = 0;
    self->field_0xd28 = 0;
    self->field_0x4f8 = 0;
    self->field_0x4fc = false;
    self->field_0x500 = 0;
    self->field_0xd70 = 0;
    self->field_0xd78 = 0;
    self->field_0xd80 = 0;
    self->field_0x504 = 1;
    self->field_0x508 = 0;
    self->field_0xd9c.x = g_unk_710164779c.x;
    self->field_0xd9c.y = g_unk_710164779c.y;
    self->field_0xd9c.z = g_unk_710164779c.z;
    self->field_0xda8 = 0;
}
