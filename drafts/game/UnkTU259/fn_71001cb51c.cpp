// UnkTU259 unit: when the flag at +0x3183668 equals fn_71001598f0's result, flips it and restarts the
// counter at +0x317aa68 at 20; returns that result. Names and layouts are placeholders.
#include "game/UnkTU259.h"
#include "types.h"

u32 fn_71001598f0();

struct UnkRenderTargetA1 {
    u8 pad_0x0[0x317aa68];
    u32 field_0x317aa68;
    u8 pad_0x317aa6c[0x3183668 - 0x317aa6c];
    bool field_0x3183668;
};

bool fn_71001cb51c(UnkRenderTargetA1* target) {
    bool active = fn_71001598f0() & 1;
    bool flag = target->field_0x3183668;
    if (!active && !flag) {
        flag = true;
    } else {
        if (!flag || !active)
            return active;
        flag = false;
    }
    target->field_0x3183668 = flag;
    target->field_0x317aa68 = 20;
    return active;
}
