// From unit 53 of tools/tu_map.py (0x7100075f78-0x71000769f0, no reflected class): the shaping of a
// curve parameter by a string of modifier letters, which the unit's other functions and dozens of
// callers apply to a 0..1 value. Each letter reshapes the value in turn ('2' squares it, 'i'
// inverts it, 'q' takes its root, 'R' and 'r' scale it by a random factor ...); some use the
// parameter itself as well. Letters without a meaning are skipped. Names are placeholders.
// Holds fn_7100076020.
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
