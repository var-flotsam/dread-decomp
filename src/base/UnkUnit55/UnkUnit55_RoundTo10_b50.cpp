// Unit 55 of tools/tu_map.py: rounds both components of a 2D float vector to the nearest multiple of 10
// (halves away from zero). Names are placeholders.
#include "types.h"

struct UnkVec2fR {
    f32 x, y;
};

static inline f32 RoundTo10(f32 v) {
    f32 t = v / 10.0f;
    return (s32)(t + (t > 0.0f ? 0.5f : -0.5f)) * 10;
}

void fn_710007b458(UnkVec2fR* v) {
    f32 x = RoundTo10(v->x);
    f32 y = RoundTo10(v->y);
    v->x = x;
    v->y = y;
}

// Rounds each component of a 3D float vector to the nearest integer (halves away from zero).
struct UnkVec3fR {
    f32 x, y, z;
};

static inline f32 RoundHalfAway(f32 v) {
    return v >= 0.0f ? __builtin_floorf(v + 0.5f) : __builtin_ceilf(v - 0.5f);
}

void fn_710007b4b0(UnkVec3fR* v) {
    v->x = RoundHalfAway(v->x);
    v->y = RoundHalfAway(v->y);
    v->z = RoundHalfAway(v->z);
}
