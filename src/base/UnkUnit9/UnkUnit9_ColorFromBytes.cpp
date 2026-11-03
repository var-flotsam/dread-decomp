// tools/tu_map.py unit 9 (no class hint): unpacks four bytes (x lowest) into a float RGBA colour,
// each channel scaled by 1/255; the inverse of fn_7100002c40. Names are placeholders.
#include "types.h"

struct UnkColorBytes4f {
    f32 r, g, b, a;
};

UnkColorBytes4f fn_7100002c00(u32 packed) {
    const f32 scale = 1.0f / 255.0f;
    UnkColorBytes4f c;
    c.r = (f32)(packed & 0xff) * scale;
    c.g = (f32)((packed >> 8) & 0xff) * scale;
    c.b = (f32)((packed >> 16) & 0xff) * scale;
    c.a = (f32)(packed >> 24) * scale;
    return c;
}
