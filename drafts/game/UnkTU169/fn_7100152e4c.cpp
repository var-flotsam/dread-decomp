// Unit 169: evaluates four Hermite key-frame curves at a time and stores the four results.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHermiteKey {
    f32 time;
    f32 inTangent;
    f32 outTangent;
    f32 value;
};

struct UnkHermiteTrack {
    u32 count;
    u8 pad_0x4[8];
    f32 defaultValue;
    UnkHermiteKey* keys;

    f32 Evaluate(f32 t) const {
        if (count == 0 || keys == nullptr)
            return defaultValue;
        if (keys[0].time >= t)
            return keys[0].value;
        u32 last = count - 1;
        const UnkHermiteKey* next = &keys[last];
        if (next->time <= t)
            return next->value;

        const UnkHermiteKey* prev = keys;
        u32 lo = 0;
        u32 hi = last;
        while (lo != hi) {
            u32 mid = (hi + lo) / 2;
            const UnkHermiteKey* key = &keys[mid];
            if (key->time > t) {
                hi = mid;
                next = key;
            } else {
                lo = mid + 1;
                prev = key;
            }
        }

        f32 span = next->time - prev->time;
        f32 dt = t - prev->time;
        f32 u = dt / span;
        f32 um1 = u - 1.0f;
        f32 base = prev->value + u * (u * ((prev->value - next->value) * (u + u - 3.0f)));
        return dt * um1 * (um1 * prev->outTangent + u * next->inTangent) + base;
    }
};

struct UnkHermiteCurve4 {
    f32 values[4];
    UnkHermiteTrack tracks[4];
};

void fn_7100152e4c(f32 t, UnkHermiteCurve4* curve) {
    curve->values[0] = curve->tracks[0].Evaluate(t);
    curve->values[1] = curve->tracks[1].Evaluate(t);
    curve->values[2] = curve->tracks[2].Evaluate(t);
    curve->values[3] = curve->tracks[3].Evaluate(t);
}
