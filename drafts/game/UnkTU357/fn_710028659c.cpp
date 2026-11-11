// Unit 357: resets a state block to its default values (zeroed, ids -1, floats -1.0 / 1.0).
// Names and layouts are placeholders.
#include "types.h"

struct UnkPair {
    u64 a, b;
};

struct __attribute__((packed)) UnkResetBlock {
    u64 field_0x0;
    UnkPair field_0x8[7];
    u16 field_0x78;
    u8 field_0x7a;
    u8 pad_0x7b;
    u32 field_0x7c;
    u8 field_0x80;
    u8 pad_0x81[3];
    u64 field_0x84;
    u16 field_0x8c;
    u8 pad_0x8e[2];
    u32 field_0x90;
    u8 pad_0x94[0xc0];
    UnkPair field_0x154;
    u64 field_0x164;
    u64 field_0x16c;
    u8 pad_0x174[4];
    UnkPair field_0x178[2];
    u64 field_0x198;
    u8 field_0x1a0;
    u8 pad_0x1a1[3];
    u64 field_0x1a4;
    u32 field_0x1ac;
    u8 field_0x1b0;
};

void fn_710028659c(UnkResetBlock* self) {
    UnkPair zero = {0, 0};
    self->field_0x78 = 0xffff;
    self->field_0x8c = 0xffff;
    self->field_0x0 = 0;
    self->field_0x7a = 0xff;
    self->field_0x7c = 0;
    self->field_0x80 = 0;
    self->field_0x84 = 0;
    self->field_0x90 = 0;
    for (int i = 6; i >= 0; i--) {
        self->field_0x8[i] = zero;
    }
    self->field_0x154 = zero;
    self->field_0x164 = 0xbf800000;
    self->field_0x16c = 0x3f800000;
    self->field_0x1a4 = 0;
    self->field_0x198 = 0;
    self->field_0x1a0 = 0;
    self->field_0x1ac = 0xbf800000;
    self->field_0x1b0 = 0;
    self->field_0x178[0] = zero;
    self->field_0x178[1] = zero;
}
