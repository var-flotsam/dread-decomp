// tools/tu_map.py unit 110: whether two 4-component float vectors are equal within 0.001 per component.
// Names are placeholders.
#include "types.h"

struct UnkVec4fE {
    f32 x, y, z, w;
};

static inline bool NearlyEqual(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

bool fn_7100125b70(const UnkVec4fE* a, const UnkVec4fE* b) {
    return NearlyEqual(a->x, b->x) && NearlyEqual(a->y, b->y) && NearlyEqual(a->z, b->z) &&
           NearlyEqual(a->w, b->w);
}
