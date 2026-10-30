// From unit 53 of tools/tu_map.py (0x7100075f78-0x71000769f0, no reflected class): the shaping of a
// curve parameter by a string of modifier letters, which the unit's other functions and dozens of
// callers apply to a 0..1 value. Each letter reshapes the value in turn ('2' squares it, 'i'
// inverts it, 'q' takes its root, 'R' and 'r' scale it by a random factor ...); some use the
// parameter itself as well. Letters without a meaning are skipped. Names are placeholders.
// Holds fn_7100076020 and fn_71000763c0 (a timed curve's step).
#include "types.h"

extern "C" int rand();
extern "C" f32 sqrtf(f32);

// A random factor in 0..1.
static inline f32 Random01() {
    return (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
}

// A random value between `lo` and `hi`.
static inline f32 RandomRange(f32 lo, f32 hi) {
    f32 r = Random01();
    return lo + (hi - lo) * r;
}

// `t` reshaped by each letter of `modifiers` in turn; at t = 1 a result within 1e-5 of 0 or 1 is
// snapped to it.
f32 fn_7100076020(f32 t, const char* modifiers) {
    f32 v = t;
    const char* p = modifiers;
    while (char c = *p++) {
        // The parameter's tent (0 at both ends, 1 in the middle) and its complement.
        f32 rest = 1.0f - t;
        f32 tent = t < 0.5f ? t + t : 2.0f - (t + t);
        switch (c) {
        case 'C':
            v = tent * (v + v) - tent;
            break;
        case 'I':
            v = rest * v;
            break;
        case 'O':
            v = tent * v;
            break;
        case 'P':
            v = rest * (v + v) - rest;
            break;
        case 'R':
            v = RandomRange(-v, v);
            break;
        case 'S': {
            f32 half = v * 0.5f;
            v = v < 0.5f ? sqrtf(half) : 1.0f - sqrtf(0.5f - half);
            break;
        }
        case 'a':
            v = v * v * (2.0f - v);
            break;
        case 'd':
            v = v < 0.1f ? 1.0f : v * 1.1f - 0.1f;
            break;
        case 'i':
            v = 1.0f - v;
            break;
        case 'o':
            v = v < 0.5f ? v + v : 2.0f - (v + v);
            break;
        case 'q':
            v = sqrtf(v);
            break;
        case 'r':
            v = RandomRange(0.0f, v);
            break;
        case 's':
            v = v < 0.5f ? 2.0f * v * v : 4.0f * v - 2.0f * v * v - 1.0f;
            break;
        case 'z':
            v = v + v - 1.0f;
            break;
        case '2':
            v = v * v;
            break;
        }
    }
    if (t == 1.0f) {
        if (v > -1e-5f && v < 1e-5f)
            v = 0.0f;
        else if (v > 0.99999f && v < 1.00001f)
            v = 1.0f;
    }
    return v;
}

// A value that runs from `start` to `start + delta` over `duration`, shaped by `modifiers`; `end`
// is what it reports once the time has run out.
struct UnkTimedCurve {
    f32 start;
    f32 field_0x4;
    f32 duration;
    f32 delta;
    f32 time;
    u32 field_0x14;
    const char* modifiers;
    u8 field_0x20[0x28];
    f32 end;
};

// Advances the curve's time by `dt`, clamped to 0..duration, and returns its value there.
// NON_MATCHING(minor): the original shares one copy of the shaping tail between the two clamp
// branches; ours duplicates it for the branch that starts at 0.
f32 fn_71000763c0(f32 dt, UnkTimedCurve* curve) {
    f32 t = curve->time + dt;
    if (t <= 0.0f) {
        t = 0.0f;
        curve->time = t;
        if (t < curve->duration)
            goto shape;
    } else {
        f32 duration = curve->duration;
        t = t >= duration ? duration : t;
        curve->time = t;
        if (t < duration)
            goto shape;
    }
    return curve->end;
shape:
    f32 u = t / curve->duration;
    if (curve->modifiers && *curve->modifiers)
        u = fn_7100076020(u, curve->modifiers);
    return curve->start + u * curve->delta;
}

// ---- fn_7100075f78: setting a timed curve up ----

#include "base/global/CRntString.h"

// Sets the curve to run from `start` to `target` over `duration`, taking the modifier letters from
// `modifiers` when they differ, and works out the value it ends at.
// NON_MATCHING(minor): the stores of the duration and the difference come out in the other order.
void fn_7100075f78(f32 start, f32 target, f32 duration, UnkTimedCurve* curve,
                   const base::global::CRntString* modifiers) {
    // The modifier letters are the data of a string held at +0x18.
    auto* letters = reinterpret_cast<base::global::CRntString*>(&curve->modifiers);
    curve->start = start;
    curve->field_0x4 = target;
    curve->duration = duration;
    curve->delta = target - start;
    curve->time = 0.0f;
    if (fn_710007f220(letters, modifiers))
        fn_710007eaa0(letters, modifiers);
    if (curve->modifiers && *curve->modifiers) {
        f32 f = fn_7100076020(1.0f, curve->modifiers);
        curve->end = curve->start + f * curve->delta;
        return;
    }
    curve->end = curve->field_0x4;
}
