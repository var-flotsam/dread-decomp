// UnkTU259 unit: when the flag at +0x3183668 equals fn_71001598f0's result, flips it and restarts the
// counter at +0x317aa68 at 20; returns that result. Names and layouts are placeholders.
#include "game/UnkRenderTargetA1.h"
#include "game/UnkTU259.h"
#include "types.h"

u32 fn_71001598f0();

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
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
