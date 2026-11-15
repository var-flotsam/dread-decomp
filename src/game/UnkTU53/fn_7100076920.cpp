// Unit 53: unpacks four bytes (x lowest) into a float RGBA colour scaled by 1/255, returned in memory.
// Names and layouts are placeholders.
#include "types.h"

struct UnkColor4f_076920 {
    f32 r, g, b, a;
    UnkColor4f_076920(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
    UnkColor4f_076920(const UnkColor4f_076920& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkColor4f_076920 fn_7100076920(u32 packed) {
    const f32 scale = 1.0f / 255.0f;
    f32 r = (f32)(packed & 0xff);
    f32 g = (f32)((packed >> 8) & 0xff);
    f32 b = (f32)((packed >> 16) & 0xff);
    f32 a = (f32)(packed >> 24);
    return UnkColor4f_076920(r * scale, g * scale, b * scale, a * scale);
}
