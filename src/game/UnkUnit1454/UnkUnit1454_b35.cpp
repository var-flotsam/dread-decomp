// A function in an unnamed unit (tu_map unit 1454, 0x7100982c7c-0x710098834c): when the absolute value of
// the float at +0x20 of the argument is 1 within 1e-5, the flag at +0x3e8 is set. Always returns true.
// Names and layouts are placeholders.
#include "types.h"

struct UnkUnit1454Owner {
    u8 unk_0x0[0x3e8];
    bool field_0x3e8;
};
struct UnkUnit1454Arg {
    u8 unk_0x0[0x20];
    f32 field_0x20;
};

bool fn_7100983454(UnkUnit1454Owner* self, const UnkUnit1454Arg* arg) {
    f32 value = __builtin_fabsf(arg->field_0x20);
    f32 low = value + -1e-5f;
    f32 high = value + 1e-5f;
    if (high >= 1.0f && low <= 1.0f)
        self->field_0x3e8 = true;
    return true;
}
