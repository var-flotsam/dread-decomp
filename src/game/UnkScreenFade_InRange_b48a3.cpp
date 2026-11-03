// From unit 2656 of tools/tu_map.py (the screen fade, src/game/UnkScreenFade.cpp): whether `t` falls in
// the fade window of the node at +0x0 (start +0x128, end +0x12c) while nothing is pending (+0x28).
// A window that starts at 0 with a non-zero end always holds. Names and layout are placeholders.
#include "types.h"

struct UnkFadeWindow_b48a3 {
    u8 pad_0x0[0x128];
    f32 start;
    f32 end;
};

struct UnkScreenFadeState_b48a3 {
    UnkFadeWindow_b48a3* window;
    u8 pad_0x8[0x20];
    s32 pending;
};

bool fn_7101053a1c(UnkScreenFadeState_b48a3* self, f32 t) {
    if (self->pending != 0)
        return false;
    UnkFadeWindow_b48a3* window = self->window;
    f32 start = window->start;
    if (start == 0.0f && window->end != 0.0f)
        return true;
    if (t > 0.0f && start < t && window->end >= t)
        return true;
    return false;
}
