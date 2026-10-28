// CEntity (tools/tu_map.py unit 2523, fn_7100d70c1c to fn_7100d8b794). This file holds
// fn_7100d7b540: the point on the ground below an entity, found by casting a segment 2000 units
// straight down through the collision tracker of the game manager's +0x608 and lifted by the
// height of the entity's movement shape.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// A position returned through x8: its copy constructor is not trivial.
struct UnkGroundPoint : CVector3D {
    UnkGroundPoint(f32 x_, f32 y_, f32 z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    UnkGroundPoint(const UnkGroundPoint& o) {
        x = o.x;
        y = o.y;
        z = o.z;
    }
};

// A reference to a counted object (base::UnkRefCounted), released when the count drops to zero.
struct UnkGroundHitRef {
    base::UnkRefCounted* ptr;

    ~UnkGroundHitRef() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// The segment query's callback object (the closest hit so far, 0x48 bytes from +0x8, and the
// segment's origin); the vtables, reached through the GOT, are the callback's and its base's.
extern void* g_unk_7101a20fa8[];
extern void* g_unk_7101a20ed8[];
struct UnkGroundHitQuery {
    void** vtable;
    UnkGroundHitRef object;
    UnkGroundHitRef field_0x10;
    s32 id;
    f32 point_x, point_y, point_z;
    f32 field_0x28[6];
    u32 field_0x40[3];
    u32 pad_0x4c;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkGroundHitQuery(const CVector3D& o)
        : id(-1), point_x(0.0f), point_y(0.0f), point_z(0.0f), field_0x28(), field_0x40(), field_0x50(false),
          origin(o), field_0x68(0) {
        object.ptr = nullptr;
        field_0x10.ptr = nullptr;
        vtable = g_unk_7101a20fa8 + 2;
    }
    ~UnkGroundHitQuery() { vtable = g_unk_7101a20ed8 + 2; }
};

// A point projected onto the plane (fn_710007b430 builds it in place).
struct UnkGroundPoint2D : CVector2D {
    explicit UnkGroundPoint2D(const CVector3D& v) { fn_710007b430(this, &v); }
};

// A collision shape (0xc0 bytes) built from a segment and its destructor, and the collision query
// (declared as src/game/CLogicDroppedItemManager.cpp does).
struct UnkSegmentShape {
    u8 data[0xc0];
};
void fn_7100d693ac(UnkSegmentShape* self, const UnkSegment2D* seg);
void fn_7100d69400(UnkSegmentShape* self);
struct UnkTree;
struct UnkShape;
struct UnkMtx44;
bool fn_7100d5b484(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u16 arg2);

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

// The entity's movement component at +0x1f8 (declared as src/game/CChozoCommanderTriComboAttack/fn_710043c084.cpp does):
// fn_7100976588 gives its shape and fn_7100977bec (CCharacterMovement unit) the height to lift a
// ground point by when it has one; else its vf_0x2a8 may give the movement object, whose shape at
// +0x158 gives a bounding box.
struct UnkFlagged;
UnkShape* fn_7100976588(UnkFlagged* flagged);
f32 fn_7100977bec(UnkFlagged* flagged);

struct UnkGroundBounds {
    CVector3D min;
    CVector3D max;
};
struct UnkGroundMoverShape {
    void** vtable;
    u8 field_0x8[1];

    UnkGroundBounds GetBounds() {
        return reinterpret_cast<UnkGroundBounds (*)(UnkGroundMoverShape*, void*)>(vtable[0x28 / 8])(this, field_0x8);
    }
};
struct UnkGroundMover {
    u8 pad_0x0[0x158];
    UnkGroundMoverShape* shape;
};
static inline UnkGroundMover* GetMover(UnkFlagged* flagged) {
    void** vtable = *reinterpret_cast<void***>(flagged);
    return reinterpret_cast<UnkGroundMover* (*)(UnkFlagged*)>(vtable[0x2a8 / 8])(flagged);
}

// The entity's +0x228: a height at +0x60.
struct UnkGroundHeight {
    u8 pad_0x0[0x60];
    f32 field_0x60;
};

struct UnkGroundEntity {
    u8 pad_0x0[0x38];
    f32 transform[12];  // +0x38, 3x4 row-major: the position is the last column
    u8 pad_0x68[0x1f8 - 0x68];
    UnkFlagged* movement;  // +0x1f8
    u8 pad_0x200[0x228 - 0x200];
    UnkGroundHeight* field_0x228;
    u8 pad_0x230[0x378 - 0x230];
    u32 collision_mask;  // +0x378
};

// Lifts a ground point by the height the entity stands at, when something gives one.
static inline void LiftToStandingHeight(UnkGroundEntity* self, CVector3D* p) {
    if (self->movement) {
        if (fn_7100976588(self->movement)) {
            p->y = fn_7100977bec(self->movement) + p->y;
            return;
        }
        if (UnkGroundMover* mover = GetMover(self->movement)) {
            UnkGroundBounds bounds = mover->shape->GetBounds();
            p->y = p->y - bounds.min.y;
            return;
        }
    }
    if (self->field_0x228)
        p->y = self->field_0x228->field_0x60 + p->y;
}

// The ground point below `pos` (the entity's position when null): the first hit of the segment
// down from it, lifted by the height the entity stands at; the point itself when nothing is hit.
// NON_MATCHING(minor): the original keeps the addresses of the result's y and z in x22 and x23
// (computed with writeback stores while the result is built, used again when the hit point is
// stored), so it saves two more registers (a 0x10-byte larger frame); ours addresses them from
// the result each time. The query's construction and the stores of the hit point are scheduled
// differently.
UnkGroundPoint fn_7100d7b540(UnkGroundEntity* self, const UnkGroundPoint* pos) {
    UnkGroundPoint result =
        pos ? *pos : UnkGroundPoint(self->transform[3], self->transform[7], self->transform[11]);
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    UnkSegmentShape shape;
    {
        UnkGroundPoint2D start(result);
        CVector3D below = {result.x, result.y + -2000.0f, result.z};
        UnkGroundPoint2D end(below);
        UnkSegment2D seg(start, end);
        fn_7100d693ac(&shape, &seg);
    }
    {
        UnkGroundHitQuery query(result);
        bool hit;
        {
            UnkMatrix44 identity;
            SetIdentity(&identity);
            hit = fn_7100d5b484(tree, self->collision_mask, reinterpret_cast<UnkShape*>(&shape),
                                reinterpret_cast<const UnkMtx44*>(&identity), &query, 0);
        }
        if (hit) {
            result.x = query.point_x;
            result.y = query.point_y;
            result.z = query.point_z;
            LiftToStandingHeight(self, &result);
        }
    }
    fn_7100d69400(&shape);
    return result;
}
