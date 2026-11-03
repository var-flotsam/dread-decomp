// From unit 2567 of tools/tu_map.py (no reflected class): the value of a bouncing curve (as in
// fn_7100eb7470.cpp, the mode at +0xc8) at the fraction `u`: mode 0 clamps u to [0, 1], mode 1 uses
// it as it is, mode 2 runs up to the middle and back down (fn_7100076450); other modes give 0.
// Names and layouts are placeholders.
#include "base/UnkTimedCurve.h"

f32 fn_7100076450(UnkTimedCurve* curve, f32 u);

struct UnkBouncingCurve_b51 {
    UnkTimedCurve curve;
    u8 pad_0x50[0x78];
    s32 mode;  // 0xc8
};

f32 fn_7100eb7530(UnkBouncingCurve_b51* self, f32 u) {
    switch (self->mode) {
    case 0:
        return fn_7100076450(&self->curve, !(u <= 0.0f) ? (u > 1.0f ? 1.0f : u) : 0.0f);
    case 1:
        return fn_7100076450(&self->curve, u);
    case 2:
        return fn_7100076450(&self->curve, u < 0.5f ? u * 2.0f : 1.0f - (u - 0.5f) * 2.0f);
    }
    return 0.0f;
}
