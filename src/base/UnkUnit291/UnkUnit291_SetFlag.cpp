// From unit 291 of tools/tu_map.py (0x710020d0dc-0x710020dfbc, the transform-node unit of
// src/game_s1/fn_710020d744.cpp): sets the node's flag at 0xf5; the target's byte at +0x70 becomes
// set when it is, else the inverse of the flag at 0xf7; the byte at +0x110 is the inverse of 0xf5.
// Names and layouts are placeholders.
#include "types.h"

struct UnkUnit291Target {
    u8 bytes[0x71];  // +0x70: a flag
};

struct UnkUnit291Node {
    u8 pad_0x0[0xf5];
    bool field_0xf5;
    u8 pad_0xf6;
    bool field_0xf7;
    u8 pad_0xf8[0x10];
    UnkUnit291Target* target_0x108;
    bool field_0x110;
};

void fn_710020d9e0(UnkUnit291Node* self, bool value) {
    self->field_0xf5 = value;
    self->target_0x108->bytes[0x70] = value ? true : !self->field_0xf7;
    self->field_0x110 = !self->field_0xf5;
}
