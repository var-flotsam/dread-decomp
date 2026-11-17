// CRodotukSuckAttack: whether the attack reaches the target: the target (or, when `bones` is set, the owner's
// "RightLeg003" or damage collider) lies within `max_angle` of the owner's up axis seen from `back` below it,
// beyond `reach` along that axis, and nothing in the collision tree lies between them. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkTU55.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

#include <math.h>

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkRodotukAI {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkRodotukAI*)>(vtable[0])(this);
    }
};

struct UnkSuckAttack {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x58];
    bool field_0x68;
};

struct UnkCollisionContainer;
struct UnkCollisionObject;

struct UnkSuckOwner_l66afd4 {
    u8 pad_0x0[0x210];
    void* field_0x210;
    u8 pad_0x218[0x2a0 - 0x218];
    UnkCollisionContainer* collision;  // 0x2a0
};

// A vector copied component by component.
struct UnkSuckVec_l66afd4 {
    f32 x, y, z;

    UnkSuckVec_l66afd4() {}
    UnkSuckVec_l66afd4(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, a vector, the
// normal and flags.
struct UnkSuckHit_l66afd4 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    UnkSuckVec_l66afd4 point;
    UnkSuckVec_l66afd4 field_0x20;
    UnkSuckVec_l66afd4 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkSuckHit_l66afd4()
        : object{nullptr}, field_0x8{nullptr}, id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f), field_0x38(0), flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkSuckHit_l66afd4) == 0x48, "UnkSuckHit_l66afd4 layout");

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkSuckQuery_l66afd4 {
    void** vtable;
    UnkSuckHit_l66afd4 hit;
    bool field_0x50;
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkSuckQuery_l66afd4(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSuckQuery_l66afd4() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSuckSegment_l66afd4 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSuckSegment_l66afd4(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

inline const CStrId& DamageId_l66afd4() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return s_table.field_0x1650;
}

static inline void ToPlane(CVector2D* out, const CVector3D& v) {
    fn_710007b430(out, &v);
}

// The unit direction from one point to another (zero when they meet).
static inline CVector2D Direction(const CVector2D& from, const CVector2D& to) {
    f32 x = to.x - from.x;
    f32 y = to.y - from.y;
    f32 length = sqrtf(x * x + y * y);
    if (length != 0.0f) {
        x /= length;
        y /= length;
    }
    return CVector2D{x, y};
}

// The translation of a collider's matrix.
static inline void ColliderPos(CVector2D* out, UnkCollisionObject* collider) {
    UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider));
    UnkSuckVec_l66afd4 pos;
    pos.x = m.m[3];
    pos.y = m.m[7];
    pos.z = m.m[11];
    fn_710007b444(out, reinterpret_cast<const UnkVec3*>(&pos));
}

static inline f32 AngleTo(const CVector2D& from, const CVector2D& to, const CVector2D& axis) {
    CVector2D dir = Direction(from, to);
    return acosf(dir.x * axis.x + dir.y * axis.y);
}

// Whether `target` lies beyond the point `reach` along the axis from `eye`.
static inline bool Beyond(const CVector2D& eye, const CVector2D& axis, f32 reach, const CVector2D& target) {
    CVector2D tip = {axis.x * reach + eye.x, axis.y * reach + eye.y};
    CVector2D d = {target.x - tip.x, target.y - tip.y};
    return -axis.y * d.y - axis.x * d.x > 0.0f;
}

bool fn_710066afd4(UnkSuckAttack* self, void* arg, f32 reach, f32 back, f32 max_angle, bool bones) {
    UnkRodotukAI* ai = static_cast<UnkRodotukAI*>(self->owner->field_0x240);
    if (!ai)
        return false;
    bool rodotuk = fn_710009d50c(ai->vf_0x0(), fn_7100671df8());
    UnkActor* target = static_cast<UnkActor*>(arg);
    if (!target || !rodotuk)
        return false;

    CVector2D target_pos;
    {
        f32 x = target->field_0x44;
        f32 y = target->field_0x54;
        f32 z = target->field_0x64;
        ToPlane(&target_pos, CVector3D{x, y, z});
    }
    UnkActor* owner = self->owner;
    f32 ox = owner->field_0x44;
    f32 oy = owner->field_0x54;
    f32 oz = owner->field_0x64;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    CVector2D eye;
    {
        f32 x = ox - m.m[1] * back;
        f32 y = oy - m.m[5] * back;
        f32 z = oz - m.m[9] * back;
        ToPlane(&eye, CVector3D{x, y, z});
    }
    CVector2D dir = Direction(eye, target_pos);
    const UnkMatrix44& m2 = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
    CVector2D axis;
    {
        f32 x = m2.m[1];
        f32 y = m2.m[5];
        f32 z = m2.m[9];
        ToPlane(&axis, CVector3D{x, y, z});
    }
    f32 dot = dir.x * axis.x + dir.y * axis.y;
    if (!(dot > 0.0f))
        return false;

    bool reaches;
    if (acosf(dot) < max_angle) {
        reaches = Beyond(eye, axis, reach, target_pos);
    } else {
        if (!bones)
            return false;
        if (reinterpret_cast<UnkSuckOwner_l66afd4*>(fn_7100d7c990(self->owner))->field_0x210)
            return false;
        CStrId leg("RightLeg003", true);
        CStrId chest("Chest001", true);
        UnkCollisionContainer* collision =
            reinterpret_cast<UnkSuckOwner_l66afd4*>(fn_7100d7c990(self->owner))->collision;
        UnkCollisionObject* damage = fn_71008640a4(collision, &DamageId_l66afd4());
        UnkCollisionObject* foot = fn_71008640a4(collision, &leg);
        ColliderPos(&target_pos, damage);
        if (AngleTo(eye, target_pos, axis) < max_angle) {
            reaches = Beyond(eye, axis, reach, target_pos);
        } else {
            ColliderPos(&target_pos, foot);
            reaches = AngleTo(eye, target_pos, axis) < max_angle && Beyond(eye, axis, reach, target_pos);
        }
    }
    if (!reaches)
        return false;

    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    owner = self->owner;
    CVector2D from = {owner->field_0x44, owner->field_0x54};
    CVector2D to = {target->field_0x44, target->field_0x54};
    UnkSuckSegment_l66afd4 segment(from, to);
    UnkSuckQuery_l66afd4 query(CVector3D{owner->field_0x44, owner->field_0x54, owner->field_0x64});
    return !fn_7100d5c518(tree, 0x80409, reinterpret_cast<const UnkBox*>(&segment), &query, 2);
}
