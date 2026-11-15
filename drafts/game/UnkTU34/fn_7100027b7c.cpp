// UnkTU34: cubic Hermite interpolation in a curve with u16 keys and quantised values and tangents.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPackedEntry {
    u16 value;
    u16 tangent;
};

struct UnkPackedValues {
    f32 scale;
    f32 base;
    UnkPackedEntry entries[1];

    f32 Value(s32 i) const { return base + scale * ((f32)entries[i].value * (1.0f / 65536.0f)); }
    f32 Tangent(s32 i) const { return (f32)entries[i].tangent * (1.0f / 1024.0f) - 32.0f; }
};

struct UnkPackedCurve {
    u16 field_0x0;
    u16 count;

    u16* Keys() { return (u16*)(this + 1); }
    UnkPackedValues* Values() {
        return (UnkPackedValues*)(((u64)(Keys() + count) + 3) & ~(u64)3);
    }
};

f32 fn_7100027b7c(UnkPackedCurve* curve, f32 x) {
    u32 n = curve->count;
    u16* keys = curve->Keys();
    UnkPackedValues* values = curve->Values();
    s32 idx = (s32)((u64)n - 1);
    if ((f32)keys[(u32)idx] <= x)
        return values->Value(idx);
    if ((f32)keys[0] >= x)
        return values->Value(0);
    s32 lo = 0, hi = n;
    while (lo < hi) {
        s32 mid = (hi + lo) / 2;
        if ((f32)keys[(u32)mid] > x)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx = hi - 1;
    f32 k0 = keys[(u32)idx];
    f32 delta = x - k0;
    f32 t = delta / ((f32)keys[(u32)hi] - k0);
    f32 v0 = values->Value(idx);
    f32 v1 = values->Value(hi);
    f32 m0 = values->Tangent(idx);
    f32 m1 = values->Tangent(hi);
    f32 u = t - 1.0f;
    f32 pos = v0 + t * (t * ((t + t - 3.0f) * (v0 - v1)));
    return delta * u * (u * m0 + t * m1) + pos;
}
