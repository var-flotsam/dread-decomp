// tools/tu_map.py unit 257 (no class hint): stores the mode at +0xd8; modes 0 and 1 also set up the
// part held at +0x78 (+0x140: 0 and 6, then 7 for mode 0 or 1 for mode 1) and refresh it with
// fn_710019dbf4 (src/game/UnkUnit224/fn_710019db08.cpp). Units 255 and 257 hold the same
// code for different layouts. Names are placeholders.
#include "types.h"

struct UnkModePart257 {
    u8 pad_0x0[0x140];
    s32 field_a;
    s32 field_b;
    s32 field_c;
};

struct UnkModeOwner257 {
    u8 pad_0x0[0x78];
    UnkModePart257* part;  // 0x78
    u8 pad_after_part[0xd8 - 0x78 - 8];
    s32 mode;  // 0xd8
};

struct UnkBlendLayer224;  // src/game/UnkUnit224/fn_710019db08.cpp
void fn_710019dbf4(UnkBlendLayer224* self);

void fn_71001c465c(UnkModeOwner257* self, s32 mode) {
    self->mode = mode;
    s32 value;
    if (mode == 0) {
        value = 7;
    } else if (mode == 1) {
        value = 1;
    } else {
        return;
    }
    UnkModePart257* part = self->part;
    part->field_a = 0;
    part->field_b = 6;
    part->field_c = value;
    fn_710019dbf4(reinterpret_cast<UnkBlendLayer224*>(part));
}
