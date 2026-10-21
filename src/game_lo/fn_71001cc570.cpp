// Applies three stored settings and three flags through fn_7100159660, unless both of the
// first two flags are clear and the third setting is zero.
#include "types.h"

void fn_71001597a0(u32 value);
void fn_71001597f0(f32 value);
void fn_7100159790(u32 value);
void fn_7100159660(bool a, bool b, bool c);

struct UnkStruct_71001cc570 {
    u8 pad_0x0[0x50];
    u32 field_0x50;
    f32 field_0x54;
    u32 field_0x58;
    u8 pad_0x5c[0x3183607];
    u8 field_0x3183663;
    u8 field_0x3183664;
    bool field_0x3183665;
};

void fn_71001cc570(UnkStruct_71001cc570* self) {
    u8 a = self->field_0x3183663;
    u8 b = self->field_0x3183664;
    if (!a) {
        if (b)
            b = 1;
        else if (self->field_0x58 != 0)
            b = 0;
        else
            return;
    }
    bool x = a != 0;
    bool y = b != 0;
    fn_71001597a0(self->field_0x50);
    fn_71001597f0(self->field_0x54);
    fn_7100159790(self->field_0x58);
    fn_7100159660(x, y, self->field_0x3183665);
}
