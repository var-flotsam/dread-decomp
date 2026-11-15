// UnkTU259 unit: sets the flag at +0x3183668 to the opposite of fn_71001598f0's result when they agree,
// and restarts the counter at +0x317aa68 at 20. Names and layouts are placeholders.
#include "types.h"

u32 fn_71001598f0();

struct UnkTU259Toggle_l1cb51c {
    u8 pad_0x0[0x317aa68];
    u32 field_0x317aa68;
    u8 pad_0x317aa6c[0x3183668 - 0x317aa6c];
    bool field_0x3183668;
};

u32 fn_71001cb51c(UnkTU259Toggle_l1cb51c* self) {
    u32 result = fn_71001598f0();
    bool flag = self->field_0x3183668;
    bool next;
    if ((result & 1) || flag) {
        if (!flag || !(result & 1))
            return result & 1;
        next = false;
    } else {
        next = true;
    }
    self->field_0x3183668 = next;
    self->field_0x317aa68 = 20;
    return result & 1;
}
