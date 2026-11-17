// CAutsniperShootAttack's unit (tools/tu_map.py unit 576): whether the shot from the attack's origin
// (+0x58), aimed along the movement's facing, reaches the target: a segment query from the muzzle toward
// the target (or toward three of the owner's own collision bones) whose first hit lies within twice the
// owner's radius of that point. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

// As src/game/CAutsniperShootAttack/CAutsniperShootAttack_Modes_ovb0122.cpp defines them.
struct UnkAutsniperVec3_ovb0122 {
    u32 x, y, z;
};

struct UnkAutsniperMovement_ovb0122 {
    u8 pad_0x0[0x971];
    u8 field_0x971;
    u8 pad_0x972[0x9a];
    u8 field_0xa0c;
};
static_assert(__builtin_offsetof(UnkAutsniperMovement_ovb0122, field_0xa0c) == 0xa0c, "layout");

struct UnkAutsniperOwner_ovb0122 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x178];
    UnkReflObject* movement;  // 0x1f0, a CEnemyMovement
    u8 pad_0x1f8[0x20];
    UnkAutsniperMovement_ovb0122* field_0x218;
    void* anim;  // 0x220
    u8 pad_0x228[0x18];
    UnkReflObject* ai;  // 0x240, a CAutsniperAIComponent
};
static_assert(__builtin_offsetof(UnkAutsniperOwner_ovb0122, ai) == 0x240, "layout");

struct UnkAutsniperParams_ovb0122 {
    u8 pad_0x0[0xfc];
    f32 field_0xfc;
    u8 pad_0x100[0x24];
    f32 wait_time;    // 0x124
    f32 field_0x128;  // 0x128
};

struct UnkAutsniperShootAttack_ovb0122 {
    void** vtable;
    UnkAutsniperOwner_ovb0122* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkAutsniperParams_ovb0122* params;  // 0x28
    u8 pad_0x30[0x20];
    u32 state;  // 0x50
    u32 mode;   // 0x54
    u8 field_0x58[0xc];
    f32 position[3];  // 0x64
    u8 pad_0x70[0xc];
    UnkAutsniperVec3_ovb0122 target;  // 0x7c
    u8 pad_0x88[0x1c];
    f32 wait_timer;    // 0xa4
    f32 active_timer;  // 0xa8
    u8 pad_0xac[0x20];
    bool active;  // 0xcc
};
static_assert(__builtin_offsetof(UnkAutsniperShootAttack_ovb0122, active) == 0xcc, "layout");

// The movement's facing on the plane.
struct UnkSniperMovement_l3d49f0 {
    u8 pad_0x0[0x9dc];
    CVector2D facing;  // 0x9dc
};

// The members of the target actor this function reads besides UnkActor's.
struct UnkSniperTarget_l3d49f0 {
    u8 pad_0x0[0x210];
    void* field_0x210;  // set when the target is tracked as an actor; else its bones are aimed at
    u8 pad_0x218[0x88];
    UnkCollisionContainer* collision;  // 0x2a0
};

// A reference to a counted object, released when the count drops to zero.
struct UnkHitRef_l3d49f0 {
    base::UnkRefCounted* ptr;

    UnkHitRef_l3d49f0() : ptr(nullptr) {}
    ~UnkHitRef_l3d49f0() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

struct UnkHitVec_l3d49f0 {
    f32 x, y, z;

    UnkHitVec_l3d49f0(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two normals
// and flags.
struct UnkCollisionHit_l3d49f0 {
    UnkHitRef_l3d49f0 object;
    UnkHitRef_l3d49f0 field_0x8;
    s32 id;
    UnkHitVec_l3d49f0 point;
    UnkHitVec_l3d49f0 field_0x20;
    UnkHitVec_l3d49f0 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l3d49f0()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkCollisionHit_l3d49f0) == 0x48, "UnkCollisionHit_l3d49f0 layout");

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkSegmentHitQuery_l3d49f0 {
    void** vtable;
    UnkCollisionHit_l3d49f0 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_l3d49f0(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_l3d49f0() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkHitSegment_l3d49f0 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_l3d49f0(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// A point projected onto the plane.
static inline CVector2D Project_l3d49f0(const CVector3D& v) {
    CVector2D out;
    fn_710007b430(&out, &v);
    return out;
}

static inline f32 Distance_l3d49f0(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

static inline CVector3D Position_l3d49f0(const UnkActor* actor) {
    return CVector3D{actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline CVector3D Translation_l3d49f0(const UnkMatrix44& m) {
    return CVector3D{m.m[3], m.m[7], m.m[11]};
}

// Scales `v` to unit length (left as it is when its length is zero).
static inline void Normalize_l3d49f0(CVector2D& v) {
    f32 length = sqrtf(v.x * v.x + v.y * v.y);
    if (length != 0.0f) {
        v.x /= length;
        v.y /= length;
    }
}

// Sets *out to whether `pos` lies in the box of half-size `r` around `point`, and returns it.
static inline bool SetNear_l3d49f0(u8* out, const UnkHitVec_l3d49f0& point, const CVector3D& pos, f32 r) {
    if (pos.x <= point.x + r && point.x - r <= pos.x && pos.y <= point.y + r && point.y - r <= pos.y &&
        pos.z <= point.z + r && point.z - r <= pos.z) {
        *out = true;
        return true;
    }
    *out = false;
    return false;
}

// The id table's string id at +0x1650.
inline const CStrId& DamageId_l3d49f0() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table.field_0x1650;
}

void fn_71003d49f0(UnkAutsniperShootAttack_ovb0122* self, u8* out) {
    UnkReflObject* movement = self->owner->movement;
    if (!movement || !fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()) || fn_71010cc3a4(movement) != 1)
        return;

    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    CVector2D dir = reinterpret_cast<UnkSniperMovement_l3d49f0*>(movement)->facing;
    Normalize_l3d49f0(dir);

    CVector2D origin = Project_l3d49f0(*reinterpret_cast<const CVector3D*>(self->field_0x58));
    f32 radius = reinterpret_cast<UnkActor*>(self->owner)->field_0x37c;
    dir.x = radius * dir.x;
    dir.y = radius * dir.y;
    f32 distance = Distance_l3d49f0(Project_l3d49f0(*reinterpret_cast<const CVector3D*>(self->pad_0x70)),
                                    Project_l3d49f0(Position_l3d49f0(reinterpret_cast<UnkActor*>(self->owner))));
    CVector2D start;
    start.x = origin.x + dir.x;
    start.y = dir.y + origin.y;
    f32 range = distance + reinterpret_cast<UnkActor*>(self->owner)->field_0x37c;

    if (reinterpret_cast<UnkSniperTarget_l3d49f0*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)))
            ->field_0x210) {
        CVector2D to = Project_l3d49f0(Position_l3d49f0(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))));
        CVector2D d;
        d.x = to.x - start.x;
        d.y = to.y - start.y;
        Normalize_l3d49f0(d);
        CVector2D end;
        end.x = start.x + range * d.x;
        end.y = start.y + range * d.y;
        UnkHitSegment_l3d49f0 segment(start, end);
        UnkSegmentHitQuery_l3d49f0 query(Position_l3d49f0(reinterpret_cast<UnkActor*>(self->owner)));
        if (fn_7100d5c518(tree, 0x80421, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
            CVector3D target = Position_l3d49f0(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
            f32 r = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x37c * 2.0f;
            if (SetNear_l3d49f0(out, query.hit.point, target, r))
                return;
        }
        return;
    }

    CStrId leg("RightLeg001", true);
    CStrId chest("Chest001", true);
    UnkCollisionContainer* collision =
        reinterpret_cast<UnkSniperTarget_l3d49f0*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)))
            ->collision;
    UnkShape* chest_shape = reinterpret_cast<UnkShape*>(fn_71008640a4(collision, &chest));
    UnkShape* damage_shape = reinterpret_cast<UnkShape*>(fn_71008640a4(collision, &DamageId_l3d49f0()));
    UnkShape* leg_shape = reinterpret_cast<UnkShape*>(fn_71008640a4(collision, &leg));

    CVector2D to = Project_l3d49f0(Translation_l3d49f0(fn_7100d36a2c(damage_shape)));
    CVector2D d;
    d.x = to.x - start.x;
    d.y = to.y - start.y;
    Normalize_l3d49f0(d);
    CVector2D end;
    end.x = start.x + range * d.x;
    end.y = start.y + range * d.y;
    UnkHitSegment_l3d49f0 segment(start, end);
    UnkSegmentHitQuery_l3d49f0 query(Position_l3d49f0(reinterpret_cast<UnkActor*>(self->owner)));
    if (fn_7100d5c518(tree, 0x80421, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
        CVector3D target = Translation_l3d49f0(fn_7100d36a2c(damage_shape));
        f32 r = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x37c * 2.0f;
        if (SetNear_l3d49f0(out, query.hit.point, target, r))
            return;
    }

    CVector2D to2 = Project_l3d49f0(Translation_l3d49f0(fn_7100d36a2c(chest_shape)));
    CVector2D d2;
    d2.x = to2.x - start.x;
    d2.y = to2.y - start.y;
    Normalize_l3d49f0(d2);
    CVector2D end2;
    end2.x = start.x + range * d2.x;
    end2.y = start.y + range * d2.y;
    UnkHitSegment_l3d49f0 segment2(start, end2);
    UnkSegmentHitQuery_l3d49f0 query2(Position_l3d49f0(reinterpret_cast<UnkActor*>(self->owner)));
    if (fn_7100d5c518(tree, 0x80421, reinterpret_cast<const UnkBox*>(&segment2), &query2, 2)) {
        CVector3D target = Translation_l3d49f0(fn_7100d36a2c(chest_shape));
        f32 r = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x37c * 2.0f;
        if (SetNear_l3d49f0(out, query2.hit.point, target, r))
            return;
    }

    CVector2D to3 = Project_l3d49f0(Translation_l3d49f0(fn_7100d36a2c(leg_shape)));
    CVector2D d3;
    d3.x = to3.x - start.x;
    d3.y = to3.y - start.y;
    Normalize_l3d49f0(d3);
    CVector2D end3;
    end3.x = start.x + range * d3.x;
    end3.y = start.y + range * d3.y;
    UnkHitSegment_l3d49f0 segment3(start, end3);
    UnkSegmentHitQuery_l3d49f0 query3(Position_l3d49f0(reinterpret_cast<UnkActor*>(self->owner)));
    if (fn_7100d5c518(tree, 0x80421, reinterpret_cast<const UnkBox*>(&segment3), &query3, 2)) {
        CVector3D target = Translation_l3d49f0(fn_7100d36a2c(leg_shape));
        f32 r = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x37c * 2.0f;
        SetNear_l3d49f0(out, query3.hit.point, target, r);
    }
}
