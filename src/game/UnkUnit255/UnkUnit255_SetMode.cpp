// tools/tu_map.py unit 255 (no class hint): stores the mode at +0x80; modes 0 and 1 also set up the
// part held at +0x78 (+0xd0: 0 and 6, then 7 for mode 0 or 1 for mode 1) and refresh it with
// fn_710019639c (src/game/UnkUnit224/fn_710019db08.cpp). Units 255 and 257 hold the same
// code for different layouts. Names are placeholders.
#include "types.h"

struct UnkModePart255 {
    u8 pad_0x0[0xd0];
    s32 field_a;
    s32 field_b;
    s32 field_c;
};

struct UnkModeOwner255 {
    u8 pad_0x0[0x78];
    UnkModePart255* part;  // 0x78
    u8 pad_after_part[0x80 - 0x78 - 8];
    s32 mode;  // 0x80
};

struct UnkBlendLayer_710019639c;  // src/game/UnkUnit224/fn_710019db08.cpp
void fn_710019639c(UnkBlendLayer_710019639c* self);

void fn_71001c39c8(UnkModeOwner255* self, s32 mode) {
    self->mode = mode;
    s32 value;
    if (mode == 0) {
        value = 7;
    } else if (mode == 1) {
        value = 1;
    } else {
        return;
    }
    UnkModePart255* part = self->part;
    part->field_a = 0;
    part->field_b = 6;
    part->field_c = value;
    fn_710019639c(reinterpret_cast<UnkBlendLayer_710019639c*>(part));
}
