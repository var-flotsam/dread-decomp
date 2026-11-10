// Unit 178: flips the state a bool selects (a mirrored axis: the scale is doubled and subtracted from
// one, the next word is minus twice the last, which becomes -1.0). Names and layouts are placeholders.
#include "types.h"

struct UnkFlipState_b0151 {
    u8 pad_0x0[0x28];
    f32 field_0x28;
    f32 field_0x2c;
    u8 pad_0x30[0x8];
    f32 field_0x38;
};

void fn_710015bf00(bool keep, UnkFlipState_b0151* state) {
    if (keep)
        return;
    state->field_0x28 = 1.0f - (state->field_0x28 + state->field_0x28);
    state->field_0x2c = state->field_0x38 * -2.0f;
    *reinterpret_cast<u32*>(&state->field_0x38) = 0xbf800000;
}
