// behaviortree::CInPathPatrolAction unit: whether the target is ahead of the owner inside the patrol box.
// The box around the target is turned to the slope of the floor the owner stands on (in the second mode),
// and the owner must be inside it and facing the target. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/spatial/COBox2D.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::spatial::COBox2D;

struct UnkPolygonCollection2D;
UnkSegment2D fn_710012436c(const UnkPolygonCollection2D* self, u32 handle);

// A reference to a counted object, released when the count drops to zero.
struct UnkRef_l38c4c8 {
    base::UnkRefCounted* ptr;

    UnkRef_l38c4c8() : ptr(nullptr) {}
    ~UnkRef_l38c4c8() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// The collision object a hit names: vtable slot 0x18 gives its kind (0x10 for polygons), its polygons
// at +0xa8.
struct UnkHitShape_l38c4c8 {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkPolygonCollection2D* polygons() { return reinterpret_cast<UnkPolygonCollection2D*>(field_0xa8); }
    u8 field_0xa8[1];

    s32 Kind() { return reinterpret_cast<s32 (*)(UnkHitShape_l38c4c8*)>(vtable[0x18 / 8])(this); }
};

struct UnkHitOwner_l38c4c8 {
    u8 pad_0x0[0x158];
    UnkHitShape_l38c4c8* shape;
};

struct UnkHitObject_l38c4c8 {
    u8 pad_0x0[0x10];
    UnkHitOwner_l38c4c8* owner;
};

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkHitQuery_l38c4c8 {
    void** vtable;
    UnkRef_l38c4c8 object;
    UnkRef_l38c4c8 field_0x10;
    u32 id;
    CVector3D point;
    CVector3D field_0x2c;
    CVector3D normal;
    u32 field_0x44;
    u32 flags;
    u32 field_0x4c;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkHitQuery_l38c4c8(const CVector3D& o)
        : id(-1), point{0.0f, 0.0f, 0.0f}, field_0x2c{0.0f, 0.0f, 0.0f}, normal{0.0f, 0.0f, 0.0f}, field_0x44(0),
          flags(0), field_0x4c(0), field_0x50(false), origin(o), field_0x68(0) {
        vtable = g_unk_7101a20fa8 + 2;
    }
    ~UnkHitQuery_l38c4c8() { vtable = g_unk_7101a20ed8 + 2; }

    UnkHitObject_l38c4c8* HitObject() { return reinterpret_cast<UnkHitObject_l38c4c8*>(object.ptr); }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l38c4c8 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l38c4c8(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The box as a convex shape that points are tested against.
struct UnkBoxShape_l38c4c8 {
    u8 data[0xd8];

    UnkBoxShape_l38c4c8(const COBox2D* box) asm("fn_7100d68324");
    ~UnkBoxShape_l38c4c8() asm("fn_7100d6845c");
    bool Contains(const CVector2D* point) asm("fn_7100d6884c");
};

// The owner's facing source at +0x218: vtable slot 0x370 gives its facing direction.
struct UnkFacing_l38c4c8 {
    void** vtable;

    UnkVec3f Direction() { return reinterpret_cast<UnkVec3f (*)(UnkFacing_l38c4c8*)>(vtable[0x370 / 8])(this); }
};

struct UnkPatrolMode_l38c4c8 {
    u8 pad_0x0[0xec];
    s32 mode;
};

struct UnkPatrolBlackboard_l38c4c8 {
    u8 pad_0x0[0x1610];
    UnkPatrolMode_l38c4c8* field_0x1610;
};

struct UnkInPathPatrolAction_l38c4c8 {
    u8 pad_0x0[0x18];
    UnkActor* owner;
    u8 pad_0x20[0x10];
    UnkPatrolBlackboard_l38c4c8* field_0x30;
    u8 pad_0x38[0x94];
    f32 field_0xcc;  // the box's half height
};

static inline CVector3D Position_l38c4c8(const UnkActor* actor) {
    CVector3D p;
    p.x = actor->field_0x44;
    p.y = actor->field_0x54;
    p.z = actor->field_0x64;
    return p;
}

// The position projected onto the plane.
inline CVector2D Project_l38c4c8(const CVector3D& v) {
    CVector2D p;
    fn_710007b430(&p, &v);
    return p;
}

bool fn_710038c4c8(UnkInPathPatrolAction_l38c4c8* self) {
    if (UnkActor* owner = self->owner) {
        if (UnkAnimationComponent* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220)) {
            if (fn_710117b348(animation, fn_7100d2b3a8()))
                return false;
        }
    }

    CVector2D target_2d;
    fn_710007b430(&target_2d, &static_cast<const CVector3D&>(Position_l38c4c8(fn_7100d7c990(self->owner))));
    f32 half = self->field_0xcc;
    f32 width = half + half;

    COBox2D box;
    {
        CVector2D center = Project_l38c4c8(Position_l38c4c8(fn_7100d7c990(self->owner)));
        box.center = center;
    }
    box.axis_x.x = 1.0f;
    box.axis_x.y = 0.0f;
    box.axis_y.x = -0.0f;
    box.axis_y.y = 1.0f;
    box.size.x = width;
    box.size.y = half;
    box.offset = 0.0f;
    box.angle = 0.0f;

    if (self->field_0x30->field_0x1610->mode == 2) {
        UnkActor* owner = self->owner;
        CVector2D start = {owner->field_0x44, owner->field_0x54};
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
        owner = self->owner;
        f32 reach = owner->field_0x37c + 50.0f;
        CVector2D end = {start.x - m.m[1] * reach, start.y - m.m[5] * reach};
        UnkSegment_l38c4c8 segment(start, end);
        UnkHitQuery_l38c4c8 query(Position_l38c4c8(owner));
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        if (fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
            UnkHitShape_l38c4c8* shape = query.HitObject()->owner->shape;
            s32 kind = shape->Kind();
            if (shape != nullptr && kind == 0x10) {
                UnkSegment2D edge = fn_710012436c(shape->polygons(), query.id);
                UnkVec2Result normal = fn_7100125e78(&edge);
                f32 cosine = (normal.x * 0.0f + normal.y + 0.0f) / sqrtf(normal.x * normal.x + normal.y * normal.y + 0.0f);
                f32 angle = 0.0f;
                if (cosine < 1.0f)
                    angle = cosine > -1.0f ? acosf(cosine) : 3.1415927f;
                fn_7100110664(&box, angle + box.angle);
            }
        }
    }

    UnkBoxShape_l38c4c8 area(&box);
    CVector2D owner_2d = Project_l38c4c8(Position_l38c4c8(self->owner));
    bool inside = area.Contains(&owner_2d);

    UnkActor* target = fn_7100d7c990(self->owner);
    UnkActor* owner = self->owner;
    f32 dx = target->field_0x44 - owner->field_0x44;
    f32 dy = target->field_0x54 - owner->field_0x54;
    f32 dz = target->field_0x64 - owner->field_0x64;
    f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
    dx *= inv;
    dy *= inv;
    dz *= inv;
    UnkVec3f facing = static_cast<UnkFacing_l38c4c8*>(owner->field_0x218)->Direction();
    bool ahead = !(dx * facing.x + dy * facing.y + dz * facing.z < 0.001f);
    return inside & ahead;
}
