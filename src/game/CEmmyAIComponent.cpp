// From unit 811 of tools/tu_map.py (0x7100537bf0-0x710056a378), named after the CEmmyAIComponent
// reflection data it registers: the E.M.M.I. AI component. A tunable (reached through the GOT)
// picks how one of its checks is decided. Names and layouts are placeholders; the older per-function
// sources of the unit are under src/game_s2/.
// Holds fn_710055b444.
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

// The component's members read here (the flag at +0x27720 as src/game_s2/fn_710054b2b0.cpp names it).
struct UnkEmmyAIView {
    u8 pad_0x0[0x27720];
    bool field_0x27720;
    u8 pad_0x27721[0x2b2f];
    f32 field_0x2a250;
};
static_assert(__builtin_offsetof(UnkEmmyAIView, field_0x2a250) == 0x2a250, "UnkEmmyAIView placeholder layout");

// The tunable (g_unk_71726e0aa8, game_s2/UnkChargeComponent.h) seen with the members read here: a
// threshold and the mode of the check.
struct UnkEmmyTunableView {
    u8 pad_0x0[0x78];
    f32 field_0x78;
    u8 pad_0x7c[0x84];
    s32 field_0x100;
};
static inline const UnkEmmyTunableView& Tunable() {
    return reinterpret_cast<const UnkEmmyTunableView&>(g_unk_71726e0aa8);
}

bool fn_710054ad2c(UnkEmmyAIView* self);
bool fn_7100543b30(UnkEmmyAIView* self);

// The check the tunable's mode selects: 1 always, 2 while the value at +0x2a250 is within the
// tunable's threshold, 3 when the flag at +0x27720 is set and fn_710054ad2c says no, 4 what
// fn_7100543b30 says, 5 when fn_710054ad2c says no; any other mode never.
// NON_MATCHING(minor): register allocation (the tunable's address in x9 and the mode in w8, the
// original's x8 and w9) and mode 1's `true` set before the indirect branch, where the original gives
// mode 1 its own block, so the case blocks are laid out in another order; the same tests and calls.
bool fn_710055b444(UnkEmmyAIView* self) {
    bool result;
    switch (Tunable().field_0x100) {
    case 1:
        result = true;
        break;
    case 2:
        result = self->field_0x2a250 <= Tunable().field_0x78;
        break;
    case 3:
        result = self->field_0x27720;
        if (!result)
            break;
        [[fallthrough]];
    case 5:
        result = !fn_710054ad2c(self);
        break;
    case 4:
        return fn_7100543b30(self);
    default:
        result = false;
        break;
    }
    return result;
}
