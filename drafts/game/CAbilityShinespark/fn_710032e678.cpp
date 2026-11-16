// CAbilityShinespark's unit (tools/tu_map.py): on a contact steep enough against the ability's
// direction, finds where the move crosses the contact polygon's edge (fn_71001182bc) and stops the
// movement when that point is off by more than the tolerance. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/UnkMatrix44.h"
#include "game/UnkTU109.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// Where a move from `from` to `to`, placed by `m`, meets the polygon's outline.
UnkVec2Result fn_71001182bc(UnkStruct_0x48* polygon, const CVector2D& from, const CVector2D& to,
                            const UnkMatrix44_7100078c9c& m, f32 tolerance);

struct UnkEdgePoint_32e678 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon_32e678 {
    UnkEdgePoint_32e678* points;
    u32 count;
};

struct UnkPolygons_32e678 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 GetType();
    u8 pad_0x8[0xa0];
    u8 pool[0x48];  // 0xa8
};

struct UnkShape_32e678 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual const UnkMatrix44* GetTransform();
    u8 pad_0x8[0x150];
    UnkPolygons_32e678* polygons;  // 0x158
};

struct UnkOwnerRef_32e678 {
    u8 pad_0x0[0x10];
    UnkShape_32e678* shape;
};

// The contact: its owner, the polygon and edge it is on, where it is and the surface normal.
struct UnkContact_32e678 {
    UnkOwnerRef_32e678* owner;
    u8 pad_0x8[0x8];
    u32 polygon;     // 0x10; the low half is the edge
    CVector3D pos;   // 0x14
    u8 pad_0x20[0xc];
    CVector3D normal;  // 0x2c
};

struct UnkShinespark_32e678 {
    u8 pad_0x0[0x3ac];
    CVector3D pos;
};

struct UnkShinesparkMove_32e678 {
    u8 pad_0x0[0x14];
    CVector3D velocity;
};

// A 3D point's x and y, as a temporary.
struct XY : CVector2D {
    explicit XY(const CVector3D& v) { fn_710007b430(this, &v); }
};

static inline UnkVec3f Translation(const UnkMatrix44& m) { return UnkVec3f(m.m[3], m.m[7], m.m[11]); }

static inline bool IsNear(f32 a, f32 b) {
    const f32 eps = 0.001f;
    return a - eps <= b && a + eps >= b;
}

static inline bool IsNear(const CVector2D& a, const CVector2D& b) {
    return IsNear(a.x, b.x) && IsNear(a.y, b.y);
}

void fn_710032e678(UnkShinespark_32e678* self, UnkShinesparkMove_32e678* move, const UnkContact_32e678* contact,
                   const CVector2D* dir, f32 target, f32 tolerance, f32 max_angle) {
    // The direction's side vector: the cross product with the z axis, normalised.
    const CVector3D up = {0.0f, 0.0f, 1.0f};
    const CVector3D& v = self->pos;
    f32 sx = v.y * up.z - v.z * up.y;
    f32 sy = v.z * up.x - v.x * up.z;
    f32 sz = v.x * up.y - v.y * up.x;
    f32 inv = 1.0f / (sqrtf(sz * sz + (sx * sx + sy * sy)) + 0.0001f);
    sx *= inv;
    sy *= inv;
    sz *= inv;
    f32 angle = acosf(fabsf(contact->normal.x * sx + contact->normal.y * sy + contact->normal.z * sz));
    if (!(angle >= max_angle))
        return;

    UnkPolygons_32e678* polygons = contact->owner->shape->polygons;
    if (!polygons || polygons->GetType() != 16)
        return;
    UnkPolygon_32e678* polygon = reinterpret_cast<UnkPolygon_32e678*>(
        fn_710012431c(reinterpret_cast<UnkPool_0x48*>(polygons->pool), contact->polygon));
    u16 edge = static_cast<u16>(contact->polygon);
    UnkEdgePoint_32e678* points = polygon->points;
    UnkSegment2D segment(points[edge].pos,
                         polygon->count - 1 == edge ? points[0].pos : points[static_cast<u32>(edge + 1)].pos);
    if (IsNear(XY(contact->pos), segment.start) || IsNear(XY(contact->pos), segment.end))
        return;

    UnkVec2Result hit = fn_71001182bc(reinterpret_cast<UnkStruct_0x48*>(polygon), XY(contact->pos), XY(self->pos),
                                      fn_7100078cd8(Translation(*contact->owner->shape->GetTransform())), 0.001f);
    if (fabsf(hit.x * dir->x + hit.y * dir->y - target) > tolerance) {
        move->velocity.x = 0.0f;
        move->velocity.y = 0.0f;
        move->velocity.z = 0.0f;
    }
}
