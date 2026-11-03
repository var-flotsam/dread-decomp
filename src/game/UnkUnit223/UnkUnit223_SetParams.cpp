// tools/tu_map.py unit 223 (no class hint): stores three parameters (+0xe8, +0xec, +0xf0) and derives
// the mode at +0x148 from the third and the flag +0x181: 0 or 1 with the flag set, 2 or 3 without
// (the odd value when the third parameter isn't 1). Names are placeholders.
#include "types.h"

struct UnkParams223 {
    u8 pad_0x0[0xe8];
    s32 field_0xe8;
    s32 field_0xec;
    s32 field_0xf0;
    u8 pad_0xf4[0x54];
    s32 mode;  // 0x148
    u8 pad_0x14c[0x35];
    bool field_0x181;
};

void fn_710019c258(UnkParams223* self, s32 a, s32 b, s32 c) {
    self->field_0xe8 = a;
    self->field_0xec = b;
    self->field_0xf0 = c;
    if (self->field_0x181) {
        self->mode = c != 1 ? 1 : 0;
    } else {
        self->mode = c != 1 ? 3 : 2;
    }
}
