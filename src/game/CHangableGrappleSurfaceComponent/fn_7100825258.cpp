// CHangableGrappleSurfaceComponent unit (tools/tu_map.py unit 1283): the hang point for a grapple
// message, the nearest to the message's point among the polygon colliders of the owner's collision
// (mask 0x20000). Names and layouts are placeholders.
#include <functional>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/UnkMatrix44.h"
#include "game/UnkPolygonEdgeQuery.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CRntVector;
using base::global::UnkStringData;
using base::math::CVector2D;
using base::math::CVector3D;

// Shared with src/game/CHangableGrappleSurfaceComponent/CHangableGrappleSurfaceComponent_b50.cpp.
struct UnkHangPoint_b50 {
    f32 x, y, z;

    UnkHangPoint_b50(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHangPoint_b50(const UnkHangPoint_b50& other) : x(other.x), y(other.y), z(other.z) {}
    UnkHangPoint_b50& operator=(const UnkHangPoint_b50& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkHangSource_b50 {
    u8 pad_0x0[0x20];
    UnkStringData* field_0x20;
};

struct UnkHangSender_b50 {
    u8 pad_0x0[0x10];
    UnkHangSource_b50* field_0x10;
};

struct UnkHangMessage_b50 {
    UnkHangSender_b50* sender;
    u8 pad_0x8[0xc];
    UnkHangPoint_b50 point;  // 0x14
};

struct UnkHangDef_b50 {
    u8 pad_0x0[0x70];
    UnkStringData* field_0x70;  // CStrId
    f32 field_0x78;             // the length an edge must reach to hang from its middle
};

struct UnkCollisionContainer_l864654;
struct UnkCollisionShape_l864654;

struct UnkHangOwner_l825258 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer_l864654* collision;  // 0x2a0
};

struct UnkHangable_b50 {
    u8 pad_0x0[0x30];
    UnkHangOwner_l825258* owner;  // 0x30
    u8 pad_0x38[0x50];
    UnkHangPoint_b50 point;  // 0x88
    u8 pad_0x94[0x4];
    UnkHangDef_b50* def;  // 0x98
};

// The polygon layout of src/base/spatial/UnkPolygon2D.cpp.
struct UnkPolygonVertex {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPolygon2D {
    UnkPolygonVertex* vertices;
    u32 count;
    u8 unk_0xc[0x20];
    u8 closed;
    bool box_dirty;  // 0x2d: the box below is stale
    u8 unk_0x2e[0x2];
    CVector2D box_min;  // 0x30
    CVector2D box_max;  // 0x38
    u8 unk_0x40[0x8];
};
static_assert(sizeof(UnkPolygon2D) == 0x48, "UnkPolygon2D layout");

void fn_7100864654(UnkCollisionContainer_l864654* self, u32 mask,
                   base::global::CRntVector<UnkCollisionShape_l864654*>* out, bool all);

// A collider's shape: vf_0x18 is its kind, 0x10 for a polygon.
struct UnkColliderShape_l825258 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkPolygonShape_l825258 : UnkColliderShape_l825258 {
    u8 pad_0x8[0xa0];
    const UnkPolygon2D* polygon;  // 0xa8
};

struct UnkCollider_l825258 {
    u8 pad_0x0[0x158];
    UnkColliderShape_l825258* shape;  // 0x158
};

static inline UnkPolygonShape_l825258* AsPolygonShape(UnkColliderShape_l825258* shape) {
    return shape->vf_0x18() == 0x10 ? static_cast<UnkPolygonShape_l825258*>(shape) : nullptr;
}

static inline CVector3D Translation(const UnkMatrix44& m) {
    return {m.m[3], m.m[7], m.m[11]};
}

// The point relative to origin.
inline CVector3D Offset(const UnkHangPoint_b50& p, const CVector3D& origin) {
    return CVector3D{p.x - origin.x, p.y - origin.y, p.z - origin.z};
}

// The squared distance between two points.
inline f32 DistanceSq(const UnkHangPoint_b50& a, const UnkHangPoint_b50& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
}

// The x and y of a 3D vector.
static inline void To2D(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

UnkHangPoint_b50 fn_7100825258(UnkHangable_b50* self, UnkHangMessage_b50* msg) {
    UnkHangPoint_b50 best(0.0f, 0.0f, 0.0f);
    UnkCollisionContainer_l864654* collision = self->owner->collision;
    CRntVector<UnkCollider_l825258*> colliders(0xf);
    fn_7100864654(collision, 0x20000,
                  reinterpret_cast<CRntVector<UnkCollisionShape_l864654*>*>(&colliders), false);
    f32 best_distance = 3.4028235e38f;
    for (UnkCollider_l825258*& collider : colliders) {
        const UnkPolygon2D* polygon = AsPolygonShape(collider->shape)->polygon;
        CVector3D origin = Translation(fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)));

        s32 edge = -1;
        f32 t = -1.0f;
        CVector2D closest = {0.0f, 0.0f};
        CVector2D local;
        To2D(&local, Offset(msg->point, origin));
        fn_7100117ba0(polygon, &local, &edge, &t, &closest, nullptr, false, nullptr, nullptr);

        u32 i = edge;
        const UnkPolygonVertex* v = &polygon->vertices[i];
        UnkSegment2D seg(v->pos, (polygon->count - 1 == i ? polygon->vertices : &polygon->vertices[i + 1])->pos);
        f32 length = fn_7100125fbc(&seg);
        f32 width = self->def->field_0x78;
        CVector2D p;
        if (length < width || (length + 0.001f >= width && length - 0.001f <= width)) {
            p.x = (seg.start.x + seg.end.x) * 0.5f;
            p.y = (seg.start.y + seg.end.y) * 0.5f;
        } else {
            p = closest;
        }

        UnkHangPoint_b50 candidate(origin.x + p.x, origin.y + p.y, origin.z + 0.0f);
        f32 distance = DistanceSq(msg->point, candidate);
        if (distance < best_distance) {
            best_distance = distance;
            best = candidate;
        }
    }
    colliders.Free();
    return best;
}
