// UnkTU34: per-segment mode curve (step, linear or cubic Hermite) with f32 keys and 3-float values.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCurveKey3 {
    f32 value;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkModeCurve3 {
    u32 field_0x0;
    u32 count;
    u8 modes[1];

    f32* Keys() { return (f32*)(((u64)(modes + count) + 3) & ~(u64)3); }
    UnkCurveKey3* Values() {
        u32 bytes = (count << 2) | 3;
        return (UnkCurveKey3*)(((u64)Keys() + bytes) & ~(u64)3);
    }
};

f32 fn_7100028784(void* self, const f32* input, UnkModeCurve3* curve) {
    u32 n = curve->count;
    f32* keys = curve->Keys();
    s32 last = n - 1;
    f32 x = *input;
    UnkCurveKey3* values = curve->Values();
    if (keys[(u32)last] <= x)
        return values[last].value;
    if (keys[0] >= x)
        return values[0].value;
    s32 lo = 0, hi = n;
    while (lo < hi) {
        s32 mid = (hi + lo) / 2;
        if (keys[(u32)mid] > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    s64 prev = hi - 1;
    u8 mode = curve->modes[prev];
    if (mode == 2)
        return values[prev].value;
    f32 k0 = keys[(u32)prev];
    f32 delta = x - k0;
    f32 t = delta / (keys[(u32)hi] - k0);
    if (mode == 0) {
        const UnkCurveKey3& p0 = values[prev];
        const UnkCurveKey3& p1 = values[hi];
        return p0.value * (1.0f - t) + p1.value * t;
    }
    const UnkCurveKey3& p0 = values[prev];
    const UnkCurveKey3& p1 = values[hi];
    f32 u = t - 1.0f;
    f32 pos = p0.value + t * (t * ((p0.value - p1.value) * (t + t - 3.0f)));
    f32 tan = p0.field_0x8 * u + t * p1.field_0x4;
    return delta * u * tan + pos;
}
