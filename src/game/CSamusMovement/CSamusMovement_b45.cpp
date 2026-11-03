// CSamusMovement's unit (tools/tu_map.py; the name from reflection): clears a set of the
// movement's state fields and sets the flag at +0x22f9. Names and layout are placeholders.
#include "types.h"

struct UnkVec2_b45a2 {
    f32 x;
    f32 y;
};

struct UnkSamusMovement_b45a2 {
    u8 pad_0x0[0x40];
    u64 field_0x40;
    u32 field_0x48;
    u8 pad_0x4c[0x30];
    u32 field_0x7c;
    u8 pad_0x80[0x668];
    u64 field_0x6e8;
    u32 field_0x6f0;
    u8 pad_0x6f4[0xd4];
    u32 field_0x7c8;
    u8 pad_0x7cc[0x1b2d];
    bool field_0x22f9;
    u8 pad_0x22fa[0x1a];
    UnkVec2_b45a2 field_0x2314;
    u32 pad_0x231c;
    u32 field_0x2320;
    u16 field_0x2324;
    u8 pad_0x2326[0xa];
    u64 field_0x2330;
    u32 field_0x2338;
};

void fn_71009e9acc(UnkSamusMovement_b45a2* self) {
    self->field_0x7c = 0;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x2314 = UnkVec2_b45a2{0.0f, 0.0f};
    self->field_0x2320 = 0;
    self->field_0x7c8 = 0;
    self->field_0x2324 = 0;
    self->field_0x2330 = 0;
    self->field_0x2338 = 0;
    self->field_0x22f9 = true;
    self->field_0x6e8 = 0;
    self->field_0x6f0 = 0;
}
