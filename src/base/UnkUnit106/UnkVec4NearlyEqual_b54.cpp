// tools/tu_map.py unit 106: whether two 4-component float vectors are equal within 1e-5 per component
// (fn_7100125b70 in src/game/malloc/UnkVec4NearlyEqual_b50.cpp with a smaller tolerance).
// Names are placeholders.
#include "types.h"

struct UnkVec4f_b54a1 {
    f32 x, y, z, w;
};

static inline bool NearlyEqual_b54a1(f32 a, f32 b) {
    return a - 1e-5f <= b && a + 1e-5f >= b;
}

bool fn_710010d258(const UnkVec4f_b54a1* a, const UnkVec4f_b54a1* b) {
    return NearlyEqual_b54a1(a->x, b->x) && NearlyEqual_b54a1(a->y, b->y) && NearlyEqual_b54a1(a->z, b->z) &&
           NearlyEqual_b54a1(a->w, b->w);
}
