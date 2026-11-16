// UnkTU259 unit: stores a 4-float colour, each clamped to 0..1, into slot idx (stride 0x14) at +0x317aabc;
// returns false for idx > 1. Names and layouts are placeholders.
#include "types.h"

struct UnkTU259Slot_l1d2e0c {
    f32 r, g, b, a;
    u32 field_0x10;
};

struct UnkTU259Color_l1d2e0c {
    u8 pad_0x0[0x317aabc];
    UnkTU259Slot_l1d2e0c slots[2];
};

static inline f32 Clamp01(f32 v) {
    if (!(v <= 0.0f))
        return !(v >= 1.0f) ? v : 1.0f;
    return 0.0f;
}

bool fn_71001d2e0c(UnkTU259Color_l1d2e0c* self, u32 idx, const f32* color) {
    if (idx <= 1) {
        UnkTU259Slot_l1d2e0c* slot = &self->slots[idx];
        slot->r = Clamp01(color[0]);
        slot->g = Clamp01(color[1]);
        slot->b = Clamp01(color[2]);
        slot->a = Clamp01(color[3]);
        return true;
    }
    return false;
}
