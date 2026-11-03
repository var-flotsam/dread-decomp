// Unit 39 (tools/tu_map.py): blends two four-component colours, `t` of the way from `a` to `b`
// (`a` below 0, `b` from 1 up); every component of the result is clamped to 0 from below. The
// colour is returned in memory (it has a user-written copy constructor), as in
// UnkV4ClampedOps_b47.cpp. Names are placeholders.
#include "types.h"

struct UnkColor_b53 {
    f32 r, g, b, a;
    UnkColor_b53() {}
    UnkColor_b53(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
    UnkColor_b53(const UnkColor_b53& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
};

static inline f32 Positive(f32 v) {
    return v <= 0.0f ? 0.0f : v;
}

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    if (t >= 1.0f)
        return b;
    if (t < 0.0f)
        return a;
    return (1.0f - t) * a + b * t;
}

UnkColor_b53 fn_710003088c(const UnkColor_b53& from, const UnkColor_b53& to, f32 t) {
    UnkColor_b53 v(Lerp(from.r, to.r, t), Lerp(from.g, to.g, t), Lerp(from.b, to.b, t), Lerp(from.a, to.a, t));
    UnkColor_b53 out;
    out.r = Positive(v.r);
    out.g = Positive(v.g);
    out.b = Positive(v.b);
    out.a = Positive(v.a);
    return out;
}
