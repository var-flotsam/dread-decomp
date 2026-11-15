// UnkTU34: linear interpolation in a keyed float curve.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCurve {
    u32 field_0x0;
    u32 count;

    f32* Keys() { return (f32*)(this + 1); }
    f32* Values() {
        u32 bytes = (count << 2) | 3;
        return (f32*)(((u64)Keys() + bytes) & ~(u64)3);
    }
};

f32 fn_7100028a24(void* self, const f32* input, UnkCurve* curve) {
    f32 x = *input;
    u32 n = curve->count;
    f32* keys = curve->Keys();
    f32* values = curve->Values();
    s32 last = n - 1;
    if (keys[(u32)last] <= x)
        return values[last];
    if (keys[0] >= x)
        return values[0];
    s32 lo = 0, hi = n;
    while (lo < hi) {
        s32 mid = (hi + lo) / 2;
        if (keys[(u32)mid] > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    s32 prev = hi - 1;
    f32 t = (x - keys[(u32)prev]) / (keys[(u32)hi] - keys[(u32)prev]);
    return values[hi] * t + values[prev] * (1.0f - t);
}
