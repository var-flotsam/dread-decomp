// UnkTU34: cubic Hermite interpolation in a curve with u16 keys.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHermiteKey2 {
    f32 value;
    f32 field_0x4;
};

struct UnkCurve16 {
    u16 field_0x0;
    u16 count;

    u16* Keys() { return (u16*)(this + 1); }
    UnkHermiteKey2* Values() {
        return (UnkHermiteKey2*)(((u64)(Keys() + count) + 3) & ~(u64)3);
    }
};

f32 fn_7100027a7c(UnkCurve16* curve, f32 x) {
    u32 n = curve->count;
    u16* keys = curve->Keys();
    UnkHermiteKey2* values = curve->Values();
    u64 last = n - 1;
    if ((f32)keys[(u32)last] <= x)
        return values[last].value;
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
    f32 k0 = keys[(u32)prev];
    f32 delta = x - k0;
    f32 t = delta / ((f32)keys[(u32)hi] - k0);
    const UnkHermiteKey2& p0 = values[prev];
    const UnkHermiteKey2& p1 = values[hi];
    f32 u = t - 1.0f;
    f32 pos = p0.value + t * (t * ((p0.value - p1.value) * (t + t - 3.0f)));
    f32 tan = p0.field_0x4 * u + t * p1.field_0x4;
    return delta * u * tan + pos;
}
