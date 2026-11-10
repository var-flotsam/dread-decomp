// Unit 34 (no reliable name): a value mapped into [lo, hi] by one of three modes: clamped (0), wrapped around the
// range (1) or reflected back and forth (2); any other mode leaves it alone. Names are placeholders.
#include "types.h"

extern "C" f32 fmodf(f32, f32);

f32 fn_7100027990(s32 mode, f32 t, f32 lo, f32 hi) {
    switch (mode) {
    case 0: {
        f32 r = t >= hi ? hi : t;
        if (t <= lo)
            r = lo;
        return r;
    }
    case 1:
        if (t < lo || t > hi) {
            f32 m = fmodf(t - lo, hi - lo);
            f32 base = lo;
            if (!(m >= 0.0f))
                base = hi;
            return m + base;
        }
        return t;
    case 2:
        if (t < lo || t > hi) {
            f32 d = t - lo;
            f32 w = hi - lo;
            f32 m = fmodf(d, w);
            s32 q = (s32)(d / w);
            f32 a = m < 0.0f ? -m : m;
            return (q & 1) ? hi - a : a + lo;
        }
        return t;
    }
    return t;
}
