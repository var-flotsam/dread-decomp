// Unit 2550 of tools/tu_map.py (no reflected class): a constructor that only sets defaults: two
// cleared words, -1, the floats 0.01, 3.5, 0.2, 1, 1, -1, 1, 1, 1, 0.8, 3.5, 0.2, 16.3, 13.7, 1 and
// 5, a set flag and two cleared ones. Names and the layout are placeholders.
#include "game/UnkTU2550.h"
#include "types.h"

struct UnkUnit2550Settings_b46a3 {
    u64 field_0x0;
    u32 field_0x8;
    u32 pad_0xc;
    s64 field_0x10;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    bool field_0x58;
    bool field_0x59;
    bool field_0x5a;
};

void fn_7100dde718(UnkUnit2550Settings_b46a3* self) {
    self->field_0x0 = 0;
    self->field_0x8 = 0;
    self->field_0x10 = -1;
    self->field_0x18 = 0.01f;
    self->field_0x1c = 3.5f;
    self->field_0x20 = 0.2f;
    self->field_0x24 = 1.0f;
    self->field_0x28 = 1.0f;
    self->field_0x2c = -1.0f;
    self->field_0x30 = 1.0f;
    self->field_0x34 = 1.0f;
    self->field_0x38 = 1.0f;
    self->field_0x3c = 0.8f;
    self->field_0x48 = 16.3f;
    self->field_0x4c = 13.7f;
    self->field_0x40 = 3.5f;
    self->field_0x44 = 0.2f;
    self->field_0x50 = 1.0f;
    self->field_0x54 = 5.0f;
    self->field_0x58 = true;
    self->field_0x59 = false;
    self->field_0x5a = false;
}
