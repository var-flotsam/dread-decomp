// Unit 365 ("navmesh::CNavMeshGenerator"): whether two link records are equal: the same keys and
// owner, and positions and vectors equal within 0.001 per component. Names and layouts are
// placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkLinkRecord_2a6f34 {
    u8 field_0x0;
    u8 kind_a;  // 0x1
    u16 id_a;  // 0x2
    CVector2D pos_a;  // 0x4
    CVector3D dir_a;  // 0xc
    u8 pad_0x18[4];
    u8 kind_b;  // 0x1c
    u8 field_0x1d;
    u16 id_b;  // 0x1e
    CVector2D pos_b;  // 0x20
    CVector3D dir_b;  // 0x28
    u8 pad_0x34[4];
    void* owner;  // 0x38
};
static_assert(sizeof(UnkLinkRecord_2a6f34) == 0x40, "layout");

static inline bool NearlyEqual(f32 a, f32 b) {
    return a - 0.001f <= b && a + 0.001f >= b;
}

static inline bool NearlyEqual(const CVector2D& a, const CVector2D& b) {
    return NearlyEqual(a.x, b.x) && NearlyEqual(a.y, b.y);
}

static inline bool NearlyEqual(const CVector3D& a, const CVector3D& b) {
    return NearlyEqual(a.x, b.x) && NearlyEqual(a.y, b.y) && NearlyEqual(a.z, b.z);
}

bool fn_71002a6f34(const UnkLinkRecord_2a6f34* a, const UnkLinkRecord_2a6f34* b) {
    return a->kind_a == b->kind_a && a->id_a == b->id_a && NearlyEqual(a->pos_a, b->pos_a) &&
           a->kind_b == b->kind_b && a->id_b == b->id_b && NearlyEqual(a->pos_b, b->pos_b) &&
           a->owner == b->owner && NearlyEqual(a->dir_a, b->dir_a) && NearlyEqual(a->dir_b, b->dir_b);
}
