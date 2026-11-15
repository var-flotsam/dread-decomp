// UnkTU34: cubic Hermite interpolation in a keyed float curve.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHermiteKey {
    f32 value;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkHermiteCurve {
    u32 field_0x0;
    u32 count;

    f32* Keys() { return (f32*)(this + 1); }
    UnkHermiteKey* Values() {
        u32 bytes = (count << 2) | 3;
        return (UnkHermiteKey*)(((u64)Keys() + bytes) & ~(u64)3);
    }
};

f32 fn_710002864c(void* self, const f32* input, UnkHermiteCurve* curve) {
    f32 x = *input;
    u32 n = curve->count;
    f32* keys = curve->Keys();
    UnkHermiteKey* values = curve->Values();
    s32 last = n - 1;
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
    u32 prev = hi - 1;
    f32 delta = x - keys[prev];
    f32 t = delta / (keys[(u32)hi] - keys[prev]);
    const UnkHermiteKey& p0 = values[(s32)prev];
    const UnkHermiteKey& p1 = values[(u32)hi];
    f32 u = t - 1.0f;
    f32 pos = p0.value + t * (t * ((p0.value - p1.value) * (t + t - 3.0f)));
    f32 tan = p0.field_0x8 * u + t * p1.field_0x4;
    return delta * u * tan + pos;
}
