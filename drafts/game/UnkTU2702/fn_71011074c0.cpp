// Unit 2702 (hint CAIComponent): how far the owner is along its path, measured from the nearest edge to
// a point and the edge the owner faces (its movement's velocity, or straight ahead when it stands still),
// on the side the owner faces (the enemy movement's flag, or the heading's sign) and, unless only that side
// is asked for, the other side as a negative distance when that one is nearer. Names and layouts are
// placeholders.
#include <float.h>
#include <math.h>

#include <functional>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkXformNode.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkPolygon2D;
using UnkPointFilter = std::function<bool(const CVector2D&)>;
using UnkEdgeVisitor = std::function<void(u32, const UnkSegment2D&)>;
void fn_7100117f98(const UnkPolygon2D* polygon, const CVector2D* point, s32* edge, f32* t, CVector2D* closest,
                   f32* distance, bool flag, UnkPointFilter filter, UnkEdgeVisitor visit);
bool fn_7100118274(void* p0, void* p1, void* p2, void* p3, void* p4, f32* outDistance, void* p6, void* p7);
UnkVec3f fn_710007a450(void* xform, const UnkVec3f* v);

// The address of a temporary, for the callees that take their vector by pointer.
template <class T>
static inline const T* AddressOf(const T& value) {
    return &value;
}

struct UnkPathVertex_l1074c0 {
    CVector2D pos;
    f32 field_0x8;
};

// The path the AI follows (+0x68): a 2D polygon (src/base/spatial/UnkPolygon2D.cpp).
struct UnkAIPath_l1074c0 {
    UnkPathVertex_l1074c0* vertices;
    u32 count;
    u8 pad_0xc[0x20];
    u8 closed;  // 0x2c

    u32 EdgeCount() const { return count - (!closed & (count != 0)); }
    // The end of edge `i`: the next vertex, or the first one after the last.
    const CVector2D& EdgeEnd(u32 i) const { return count - 1 == i ? vertices[0].pos : vertices[i + 1].pos; }
    UnkSegment2D Edge(u32 i) const { return UnkSegment2D(vertices[i].pos, EdgeEnd(i)); }
};

struct UnkMovementVtable_l1074c0 {
    void* unk_0x0[0x42];
    UnkVec3f (*velocity)(UnkReflObject* self);  // 0x210
};
static inline UnkVec3f Velocity(UnkReflObject* movement) {
    return (*reinterpret_cast<UnkMovementVtable_l1074c0**>(movement))->velocity(movement);
}

// The enemy movement as this reads it: which side it is turned to.
struct UnkEnemyMovement_l1074c0 {
    u8 pad_0x0[0x971];
    bool turned;  // 0x971
};

struct UnkPathOwner_l1074c0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x15c];
    UnkReflObject* movement;  // 0x1f0
};
static_assert(__builtin_offsetof(UnkPathOwner_l1074c0, movement) == 0x1f0, "layout");

struct UnkPathAI_l1074c0 {
    u8 pad_0x0[0x30];
    UnkPathOwner_l1074c0* owner;  // 0x30
    u8 pad_0x38[0x7];
    bool active;  // 0x3f
    u8 pad_0x40[0x28];
    UnkAIPath_l1074c0 path;  // 0x68
};

f32 fn_7101107958(UnkPathAI_l1074c0* self, bool side, const CVector2D* pos, const CVector2D* closest, s32 edge,
                  s32 closest_edge, f32 t, f32 closest_t);

f32 fn_71011074c0(UnkPathAI_l1074c0* self, const CVector2D* point, bool one_side, bool force) {
    if ((!self->active && !force) || !self->owner->movement || self->path.EdgeCount() == 0)
        return FLT_MAX;

    s32 point_edge = -1;
    f32 point_t = 0.0f;
    CVector2D closest = {0.0f, 0.0f};
    f32 distance = 0.0f;
    fn_7100117f98(reinterpret_cast<const UnkPolygon2D*>(&self->path), point, &point_edge, &point_t, &closest,
                  &distance, false, UnkPointFilter(), UnkEdgeVisitor());

    s32 edge = 0;
    f32 t = -1.0f;
    CVector2D here;
    fn_710007b430(&here, reinterpret_cast<const CVector3D*>(AddressOf(self->owner->xform.GetTranslation())));
    UnkVec3f velocity = Velocity(self->owner->movement);
    if (fabsf(velocity.x) < 0.001f && fabsf(velocity.y) < 0.001f && fabsf(velocity.z) < 0.001f) {
        UnkVec3f ahead(0.0f, 0.0f, 1.0f);
        velocity = fn_710007a450(&ahead, AddressOf(self->owner->xform.GetAngle()));
    }

    s32 other_edge;
    f32 other_t;
    bool found;
    {
        CVector2D from;
        fn_710007b430(&from, reinterpret_cast<const CVector3D*>(AddressOf(self->owner->xform.GetTranslation())));
        found = fn_7100118274(&self->path, &from, &edge, &t, &here, nullptr, &other_edge, &other_t);
    }
    if (found) {
        // Of the two edges found, keep the one the owner moves along most nearly.
        CVector2D dir;
        fn_710007b430(&dir, reinterpret_cast<const CVector3D*>(&velocity));
        f32 len = sqrtf(dir.x * dir.x + dir.y * dir.y);
        if (len != 0.0f) {
            dir.x /= len;
            dir.y /= len;
        }
        UnkSegment2D seg = self->path.Edge(edge);
        UnkVec2Result normal = fn_7100125e78(&seg);
        f32 facing = dir.x * normal.x + dir.y * normal.y;
        fn_7100125b44(&seg, AddressOf(self->path.Edge(other_edge)));
        normal = fn_7100125e78(&seg);
        if (fabsf(dir.x * normal.x + dir.y * normal.y) < fabsf(facing)) {
            edge = other_edge;
            t = other_t;
        }
    }

    bool side;
    UnkReflObject* movement = self->owner->movement;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) && fn_71010cc3a4(movement) == 1)
        side = reinterpret_cast<UnkEnemyMovement_l1074c0*>(movement)->turned;
    else
        side = self->owner->xform.GetAngle().y > 0.0f;

    f32 result = fn_7101107958(self, side, &here, &closest, edge, point_edge, t, point_t);
    if (!one_side) {
        f32 other = fn_7101107958(self, !side, &here, &closest, edge, point_edge, t, point_t);
        result = result > other ? -other : result;
    }
    return result;
}
