// Unit 958: whether the way from a point to the player is clear for the owner: the segment between
// them hits nothing, the owner's collision shape placed at the point does not overlap anything, and
// what the shape swept towards the player meets first is a collision component no other entity
// near the point blocks. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharacterMovement.h"
#include "game/UnkAssignedSlot.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool SweepShapeAgainstCollisionTree(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkNearSelf_l63ab04;
struct UnkNearEntity_l63ab04;
bool fn_710063ab04(UnkNearSelf_l63ab04* self, const CVector3D* pos, UnkNearEntity_l63ab04* ignore);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// A reference to a counted object (base::UnkRefCounted), released when the count drops to zero.
struct UnkHitRefA_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefA_ovb0198u() : ptr(nullptr) {}
    UnkHitRefA_ovb0198u(const UnkHitRefA_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefA_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefA_ovb0198u& operator=(const UnkHitRefA_ovb0198u& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};
struct UnkHitRefB_ovb0198u {
    base::UnkRefCounted* ptr;

    UnkHitRefB_ovb0198u() : ptr(nullptr) {}
    UnkHitRefB_ovb0198u(const UnkHitRefB_ovb0198u& other) : ptr(nullptr) { Set(other.ptr); }
    ~UnkHitRefB_ovb0198u() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
    UnkHitRefB_ovb0198u& operator=(const UnkHitRefB_ovb0198u& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
    void Set(base::UnkRefCounted* p) {
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            if (base::UnkRefCounted* old = ptr) {
                if (--old->ref_count == 0)
                    delete old;
            }
            ptr = p;
        }
    }
};

// A vector copied component by component.
struct UnkHitVec_ovb0198u {
    f32 x, y, z;

    UnkHitVec_ovb0198u() {}
    UnkHitVec_ovb0198u(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHitVec_ovb0198u(const UnkHitVec_ovb0198u& o) : x(o.x), y(o.y), z(o.z) {}
    UnkHitVec_ovb0198u& operator=(const UnkHitVec_ovb0198u& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, the
// reversed normal and the normal, and flags.
struct UnkCollisionHit_ovb0198u {
    UnkHitRefA_ovb0198u object;
    UnkHitRefB_ovb0198u field_0x8;
    s32 id;
    UnkHitVec_ovb0198u point;
    UnkHitVec_ovb0198u field_0x20;
    UnkHitVec_ovb0198u normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_ovb0198u()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
    // The hit as seen from the other side: the id kept only when flag 0 says it is set, the
    // second vector the reversed normal, flags 0x21 added.
    struct Reversed {};
    UnkCollisionHit_ovb0198u(const UnkCollisionHit_ovb0198u& o, Reversed)
        : object(o.object), field_0x8(o.field_0x8), id(o.flags & 1 ? o.id : -1), point(o.point), normal(o.normal),
          field_0x38(o.field_0x38), flags(o.flags | 0x21), field_0x40(o.field_0x40) {
        field_0x20 = UnkHitVec_ovb0198u(-normal.x, -normal.y, -normal.z);
    }
    UnkCollisionHit_ovb0198u& operator=(const UnkCollisionHit_ovb0198u& o) {
        if (this == &o)
            return *this;
        object = o.object;
        field_0x8 = o.field_0x8;
        id = o.id;
        point = o.point;
        field_0x20 = o.field_0x20;
        normal = o.normal;
        field_0x38 = o.field_0x38;
        flags = o.flags;
        field_0x40 = o.field_0x40;
        return *this;
    }
};
static_assert(sizeof(UnkCollisionHit_ovb0198u) == 0x48, "UnkCollisionHit_ovb0198u layout");

// The segment query's callback object: the closest hit so far and the segment's origin. The
// vtables (reached through the GOT) are the callback's and its base's.
struct UnkSegmentHitQuery_ovb0198u {
    void** vtable;
    UnkCollisionHit_ovb0198u hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_ovb0198u(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_ovb0198u() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkHitSegment_ovb0198u {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_ovb0198u(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// A point on the plane as a 3D point.
static inline CVector3D OnPlane_ovb0198u(const CVector2D& p) {
    return CVector3D{p.x, p.y, 0.0f};
}

// The player's transform: its rows, the position in their last column.
struct UnkPlayerXform_l63a7ec {
    u8 pad_0x0[0x38];
    f32 rows[3][4];  // 0x38
};

// The owner's movement: the collision mask its queries use.
struct UnkMovement_l63a7ec {
    u8 pad_0x0[0x3cc];
    u32 collision_mask;  // 0x3cc
};

struct UnkParams_l63a7ec {
    u8 pad_0x0[0x218];
    UnkMovement_l63a7ec* movement;  // 0x218
};

struct UnkOwner_l63a7ec {
    u8 pad_0x0[0x10];
    UnkParams_l63a7ec* params;  // 0x10
};

struct UnkSelf_l63a7ec {
    u8 pad_0x0[0xa0];
    UnkOwner_l63a7ec* owner;  // 0xa0
};

struct UnkShapeHolder_l63a7ec {
    u8 pad_0x0[0x158];
    UnkShape* shape;  // 0x158
};

struct UnkHitTarget_l63a7ec {
    u8 pad_0x0[0x200];
    void* field_0x200;
};

struct UnkHitComponent_l63a7ec : base::reflection::UnkReflObject {
    u8 pad_0x8[0x28];
    UnkHitTarget_l63a7ec* target;  // 0x30
};

struct UnkHitEntity_l63a7ec {
    u8 pad_0x0[0x10];
    UnkHitComponent_l63a7ec* component;  // 0x10
};

struct UnkHitObject_l63a7ec {
    u8 pad_0x0[0x10];
    UnkHitEntity_l63a7ec* entity;  // 0x10
};

static inline CVector3D GetPosition(const UnkPlayerXform_l63a7ec* player) {
    CVector3D p;
    p.x = player->rows[0][3];
    p.y = player->rows[1][3];
    p.z = player->rows[2][3];
    return p;
}

bool fn_710063a7ec(UnkSelf_l63a7ec* self, UnkNearEntity_l63ab04* ignore, const UnkVec3f* target) {
    UnkGameManager* gm = g_unk_7101d47310;
    UnkTree* tree = gm->field_0x608 ? static_cast<UnkTree*>(gm->field_0x608->field_0x820) : nullptr;
    if (!tree)
        return false;
    UnkPlayerXform_l63a7ec* player = reinterpret_cast<UnkPlayerXform_l63a7ec*>(fn_7100dec788(gm));
    CVector2D from;
    fn_710007b430(&from, reinterpret_cast<const CVector3D*>(target));
    CVector2D to;
    {
        CVector3D player_pos = GetPosition(player);
        fn_710007b430(&to, &player_pos);
    }
    f32 dx = to.x - from.x;
    f32 dy = to.y - from.y;
    f32 dist = sqrtf(dx * dx + dy * dy);
    CVector2D dir = {dx / dist, dy / dist};
    to.x = from.x + dist * dir.x;
    to.y = from.y + dist * dir.y;

    const f32 margin = 0.0f;
    UnkMovement_l63a7ec* movement = self->owner->params->movement;
    u32 mask = movement->collision_mask;
    CVector2D start = {from.x - dir.x * margin, from.y - dir.y * margin};
    UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(UnkVec3f(start.x, start.y, 0.0f));
    UnkShapeHolder_l63a7ec* holder = reinterpret_cast<UnkShapeHolder_l63a7ec*>(
        fn_7100976588(reinterpret_cast<UnkFlagged*>(self->owner->params->movement)));
    {
        UnkHitSegment_ovb0198u segment(to, start);
        if (fn_7100d5c518(tree, mask, reinterpret_cast<const UnkBox*>(&segment), nullptr, 2))
            return false;
    }
    if (QueryCollisionTreeForTransformedShapeOverlaps(tree, mask, holder->shape, reinterpret_cast<const UnkMtx44*>(&xf), nullptr, 0))
        return false;

    UnkSegmentHitQuery_ovb0198u query(OnPlane_ovb0198u(from));
    CVector2D sweep = {dir.x * (dist + margin), dir.y * (dist + margin)};
    u32 clear = false;
    if (SweepShapeAgainstCollisionTree(tree, mask, holder->shape, reinterpret_cast<const UnkMtx44*>(&xf),
                      reinterpret_cast<const Vec2*>(&sweep), &query, false)) {
        UnkHitObject_l63a7ec* object = reinterpret_cast<UnkHitObject_l63a7ec*>(query.hit.object.ptr);
        if (UnkHitComponent_l63a7ec* component = object->entity->component) {
            if (fn_710009d50c(component->vf_0x0(), fn_710085fc44()) && component->target->field_0x200) {
                UnkVec3f pos = *target;
                clear = !fn_710063ab04(reinterpret_cast<UnkNearSelf_l63ab04*>(self),
                                       reinterpret_cast<const CVector3D*>(&pos), ignore);
            }
        }
    }
    return clear != 0;
}
