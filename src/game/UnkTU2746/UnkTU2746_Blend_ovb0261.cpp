// Unit 2746 (tools/tu_map.py, no reliable name): a component-wise blend of two 4-float values by a
// factor in [0, 1]; a negative component means "unset" and takes the other side's. Names and layouts
// are placeholders.
#include "types.h"

struct UnkFadeColor_b0242 {
    f32 r, g, b, a;
};

// One component of a blend: a negative value is "unset" and yields the other side's.
static inline f32 BlendComponent_b0261(f32 from, f32 to, f32 t) {
    if (from < 0.0f)
        return to;
    if (to < 0.0f)
        return from;
    if (t >= 1.0f)
        return to;
    if (t < 0.0f)
        return from;
    return (1.0f - t) * from + to * t;
}

UnkFadeColor_b0242 fn_71011c1764(const UnkFadeColor_b0242* from, const UnkFadeColor_b0242* to, f32 t) {
    UnkFadeColor_b0242 out;
    out.r = BlendComponent_b0261(from->r, to->r, t);
    out.g = BlendComponent_b0261(from->g, to->g, t);
    out.b = BlendComponent_b0261(from->b, to->b, t);
    out.a = BlendComponent_b0261(from->a, to->a, t);
    return out;
}
