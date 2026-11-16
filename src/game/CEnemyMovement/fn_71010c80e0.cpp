// CEnemyMovement unit: runs the base movement init, then resets the movement state fields.
// Names and layouts are placeholders.
#include "game/CMovementComponent.h"
#include "types.h"

struct UnkEnemyMovement_l0c80e0 {
    u8 pad_0x0[0x1d8];
    u64 field_0x1d8;
    u32 field_0x1e0;
    u8 pad_0x1e4[0x6f4 - 0x1e4];
    u32 field_0x6f4;
    u8 pad_0x6f8[4];
    u8 field_0x6fc;
    u8 pad_0x6fd[0x19f0 - 0x6fd];
    u64 field_0x19f0;
    u8 pad_0x19f8[0x1bd0 - 0x19f8];
    u64 field_0x1bd0;
    u64 field_0x1bd8;
    u8 pad_0x1be0[0x1c11 - 0x1be0];
    u8 field_0x1c11;
    u8 field_0x1c12;
};

void fn_71010c80e0(UnkEnemyMovement_l0c80e0* self) {
    fn_71010d0b78(reinterpret_cast<CMovementComponent*>(self));
    self->field_0x1d8 = 0;
    self->field_0x1e0 = 0;
    self->field_0x6f4 = 0;
    self->field_0x1c12 = 0;
    self->field_0x1bd0 = 0;
    self->field_0x19f0 = 0;
    self->field_0x1bd8 = 0;
    self->field_0x6fc = 1;
    self->field_0x1c11 = 0;
}
