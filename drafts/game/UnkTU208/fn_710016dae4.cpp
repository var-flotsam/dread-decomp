// Unit 208: evaluates three Hermite key curves at a time and writes the results into a vector.
// Names and layouts are placeholders.

#include "types.h"

struct UnkVec3_710016de34;

struct UnkCurveKey_710016dae4 {
    f32 time;
    f32 tangentIn;
    f32 tangentOut;
    f32 value;
};

struct UnkCurve_710016dae4 {
    u32 count;
    u8 pad_0x4[0x8];
    f32 defaultValue;
    UnkCurveKey_710016dae4* keys;

    f32 Evaluate(f32 t) const {
        if (count == 0 || keys == nullptr)
            return defaultValue;
        if (keys[0].time >= t)
            return keys[0].value;
        u32 hi = count - 1;
        const UnkCurveKey_710016dae4* next = &keys[hi];
        if (next->time <= t)
            return next->value;
        const UnkCurveKey_710016dae4* prev = keys;
        u32 lo = 0;
        while (lo != hi) {
            u32 mid = (hi + lo) >> 1;
            const UnkCurveKey_710016dae4* key = &keys[mid];
            if (key->time > t) {
                next = key;
                hi = mid;
            } else {
                lo = mid + 1;
                prev = key;
            }
        }
        f32 span = next->time - prev->time;
        f32 d = t - prev->time;
        f32 s = d / span;
        f32 sm1 = s - 1.0f;
        f32 hermite = prev->value + s * (s * ((prev->value - next->value) * (2.0f * s - 3.0f)));
        return d * sm1 * (sm1 * prev->tangentOut + s * next->tangentIn) + hermite;
    }
};
static_assert(sizeof(UnkCurve_710016dae4) == 0x18, "");

struct UnkCurveVec3_710016dae4 {
    f32 x, y, z;
    u8 pad_0xc[0x4];
    UnkCurve_710016dae4 curves[3];
};

void fn_710016dae4(UnkVec3_710016de34* out, f32 value) {
    // The vector is the first member of the curve set that owns it.
    UnkCurveVec3_710016dae4* set = reinterpret_cast<UnkCurveVec3_710016dae4*>(out);
    set->x = set->curves[0].Evaluate(value);
    set->y = set->curves[1].Evaluate(value);
    set->z = set->curves[2].Evaluate(value);
}
