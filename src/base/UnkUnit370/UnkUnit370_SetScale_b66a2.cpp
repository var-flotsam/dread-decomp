// Unit 370 of tools/tu_map.py (no reflected class): stores a positive value (1 when it is not) and its
// reciprocal. Names and layouts are placeholders.
#include "types.h"

struct UnkScaled370_b66a2 {
    u8 pad_0x0[0xc];
    f32 value;
    f32 inverse;
};

void fn_71002b24d0(UnkScaled370_b66a2* self, f32 value) {
    if (value <= 0.0f) {
        value = 1.0f;
    }
    self->value = value;
    self->inverse = 1.0f / value;
}
