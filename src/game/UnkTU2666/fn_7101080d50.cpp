// Unit 2666 (lua_bind): reads a frame rate from the script and stores 1000 / rate as the frame timer's
// interval. Names and layouts are placeholders.
#include "game/UnkTU0.h"
#include "game/UnkTU2677.h"
#include "types.h"

struct UnkGfxFrameSource_l080d50 {
    u8 pad_0x0[0x38];
    u64 field_0x38;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
s32 fn_7101080d50(lua_State* L) {
    f32 rate;
    if (fn_71010c0270(L, "f", &rate)) {
        u32 interval = (u32)(1000.0f / rate);
        UnkGfxFrameSource_l080d50* timer = reinterpret_cast<UnkGfxFrameSource_l080d50*>(g_unk_7101d47328);
        timer->field_0x38 = interval;
    }
    return 0;
}
