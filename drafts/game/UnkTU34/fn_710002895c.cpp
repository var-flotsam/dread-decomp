// UnkTU34: step lookup in a keyed float curve (value of the last key not above the input).
// Names and layouts are placeholders.
#include "types.h"

struct UnkCurve {
    u32 field_0x0;
    u32 count;
    f32 keys[1];

    f32* Values() {
        u32 bytes = (count << 2) | 3;
        return (f32*)(((u64)keys + bytes) & ~(u64)3);
    }
};

f32 fn_710002895c(void* self, const f32* input, UnkCurve* curve) {
    f32 x = *input;
    f32* keys = curve->keys;
    u32 n = curve->count;
    s32 last = n - 1;
    f32* values = curve->Values();
    if (keys[(u32)last] <= x)
        return values[last];
    if (keys[0] >= x)
        return values[0];
    s32 lo = 0, hi = n;
    while (lo < hi) {
        s32 mid = (lo + hi) / 2;
        if (keys[(u32)mid] > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    return values[hi - 1];
}
