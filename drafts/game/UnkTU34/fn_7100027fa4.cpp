// UnkTU34: per-segment mode curve (step, linear or cubic Hermite) with u16 keys.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCurveKey16 {
    f32 value;
    f32 field_0x4;
};

struct UnkModeCurve16 {
    u16 field_0x0;
    u16 count;
    u8 modes[1];

    u16* Keys() { return (u16*)(((u64)(modes + count) + 1) & ~(u64)1); }
    UnkCurveKey16* Values() {
        return (UnkCurveKey16*)(((u64)(Keys() + count) + 3) & ~(u64)3);
    }
};

f32 fn_7100027fa4(UnkModeCurve16* curve, f32 x) {
    u32 n = curve->count;
    u16* keys = curve->Keys();
    UnkCurveKey16* values = curve->Values();
    s32 idx = (s32)((u64)n - 1);
    if (!((f32)keys[(u32)idx] <= x)) {
        if ((f32)keys[0] >= x)
            return values[0].value;
        s32 lo = 0, hi = n;
        while (lo < hi) {
            s32 mid = (hi + lo) / 2;
            if ((f32)keys[(u32)mid] > x)
                hi = mid;
            else
                lo = mid + 1;
        }
        s32 prev = hi - 1;
        idx = prev;
        u8 mode = curve->modes[idx];
        if (mode != 2) {
            f32 k0 = keys[(u32)prev];
            f32 delta = x - k0;
            f32 t = delta / ((f32)keys[(u32)hi] - k0);
            const UnkCurveKey16& p0 = values[idx];
            const UnkCurveKey16& p1 = values[hi];
            if (mode == 0)
                return p0.value * (1.0f - t) + p1.value * t;
            f32 u = t - 1.0f;
            f32 pos = p0.value + t * (t * ((p0.value - p1.value) * (t + t - 3.0f)));
            f32 tan = p0.field_0x4 * u + t * p1.field_0x4;
            return delta * u * tan + pos;
        }
    }
    return values[idx].value;
}
