// Unit 275: whether two transforms are equal: translations within 0.001 per component, then, when both
// have the derived parts the type's mask names, angles and scales within 0.001; failing that, the same
// matrix-valid flag and the same matrix bit for bit. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

extern u32 g_unk_716e597848 __attribute__((visibility("hidden")));

static inline bool NearlyEqual(f32 a, f32 b) {
    return b <= a + 0.001f && a - 0.001f <= b;
}

static inline bool NearlyEqual(const UnkVec3f& a, const UnkVec3f& b) {
    return NearlyEqual(a.x, b.x) && NearlyEqual(a.y, b.y) && NearlyEqual(a.z, b.z);
}

// Whether the transform has every derived part the type's mask names.
static inline bool HasDerived(const UnkXform* x) {
    return (x->flags & g_unk_716e597848) == g_unk_716e597848;
}

bool fn_71001f81b8(const UnkXform* a, const UnkXform* b) {
    if (!NearlyEqual(a->m[0][3], b->m[0][3]) || !NearlyEqual(a->m[1][3], b->m[1][3]) ||
        !NearlyEqual(a->m[2][3], b->m[2][3]))
        return false;
    if (HasDerived(a) && HasDerived(b) && NearlyEqual(a->angle, b->angle) && NearlyEqual(a->scale, b->scale))
        return true;
    if ((a->flags ^ b->flags) & 8)
        return false;
    return __builtin_memcmp(a->m, b->m, sizeof(a->m)) == 0;
}
