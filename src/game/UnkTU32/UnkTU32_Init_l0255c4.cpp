// Unit 32: the constructor of a member that holds four cleared words, a 0x5c-byte record (+0x28), an
// identity 4x4 matrix (+0x84), an index of -1 and three flags. Names and layouts are placeholders.
#include "base/UnkInitRecord_0x5c.h"
#include "game/UnkTU32.h"
#include "types.h"

struct UnkTU32Member_l0255c4 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u8 pad_0x20[8];
    u8 record[0x5c];  // 0x28
    f32 matrix[16];   // 0x84
    s32 field_0xc4;
    u32 field_0xc8;
    u8 field_0xcc;
    u8 field_0xcd;
    u8 field_0xce;
};
static_assert(__builtin_offsetof(UnkTU32Member_l0255c4, field_0xce) == 0xce, "UnkTU32Member_l0255c4 layout");

void fn_71000255c4(void* member) {
    UnkTU32Member_l0255c4* self = static_cast<UnkTU32Member_l0255c4*>(member);
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    fn_7100025624(self->record);
    for (int i = 0; i < 16; ++i)
        self->matrix[i] = 0.0f;
    self->matrix[15] = 1.0f;
    self->matrix[10] = 1.0f;
    self->matrix[5] = 1.0f;
    self->matrix[0] = 1.0f;
    self->field_0xc4 = -1;
    self->field_0xc8 = 0;
    self->field_0xcc = 0;
    self->field_0xcd = 1;
    self->field_0xce = 0;
}
