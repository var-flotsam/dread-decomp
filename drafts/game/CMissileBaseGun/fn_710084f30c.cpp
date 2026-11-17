// CMissileBaseGun unit: whether the gun can fire from `pos` along `dir`. It casts from the muzzle back
// along the direction (further when the gun's back-distance check finds a track), through the collision
// tree when the check applies, and fires only when the cast passes and the owner's animation is not in
// the state the id table names. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkGun_b44a2;
void fn_71008350e0(UnkGun_b44a2* self, UnkActor* actor, f32* distance, bool* found);
extern UnkGun_b44a2 g_unk_7172725ea8;

// The id table's slot at +0x9c0 (not in the shared layout yet).
struct UnkIdTableSlot_l84f30c {
    u8 pad_0x0[0x9c0];
    CStrId field_0x9c0;
};

// A reference to a counted object, released when the count drops to zero.
struct UnkRef_l84f30c {
    base::UnkRefCounted* ptr;

    UnkRef_l84f30c() : ptr(nullptr) {}
    ~UnkRef_l84f30c() {
        if (base::UnkRefCounted* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// The segment query's callback object: the closest hit so far and the segment's origin.
struct UnkHitQuery_l84f30c {
    void** vtable;
    UnkRef_l84f30c object;
    UnkRef_l84f30c field_0x10;
    s32 id;
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

    UnkHitQuery_l84f30c(const CVector2D& o)
        : id(-1), point{0.0f, 0.0f, 0.0f}, field_0x2c{0.0f, 0.0f, 0.0f}, normal{0.0f, 0.0f, 0.0f}, field_0x44(0),
          flags(0), field_0x4c(0), field_0x50(false), origin{o.x, o.y, 0.0f}, field_0x68(0) {
        vtable = g_unk_7101a20fa8 + 2;
    }
    ~UnkHitQuery_l84f30c() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkSegment_l84f30c {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkSegment_l84f30c(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// The shape whose half extent pushes the cast out of the wall.
struct UnkCastShape_l84f30c {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    u8 pad_0x44[0x4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    bool field_0x60;

    void vf_0x98() { reinterpret_cast<void (*)(UnkCastShape_l84f30c*)>(vtable[0x98 / 8])(this); }
};

struct UnkCastShapeHolder_l84f30c {
    u8 pad_0x0[0x10];
    UnkCastShape_l84f30c* shape;
};

// What the cast runs through: vtable slot 0x2b0 casts from `to` towards `from`.
struct UnkCaster_l84f30c {
    void** vtable;
    u8 pad_0x8[0x4a0];
    UnkCastShapeHolder_l84f30c* field_0x4a8;

    bool Cast(const CVector3D* to, const CVector3D* from, bool flag, bool flag2, UnkRef_l84f30c* out) {
        return reinterpret_cast<bool (*)(UnkCaster_l84f30c*, const CVector3D*, const CVector3D*, bool, bool,
                                         UnkRef_l84f30c*)>(vtable[0x2b0 / 8])(this, to, from, flag, flag2, out);
    }
    // Whether the way from rom to 	o is clear; the hit itself is dropped.
    bool IsClear(const CVector3D* to, const CVector3D* from, bool flag) {
        UnkRef_l84f30c hit;
        return Cast(to, from, flag, false, &hit);
    }
};

struct UnkMissileOwner_l84f30c {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x78];
    void* field_0x2a0;
};

struct UnkMissileGun_l84f30c {
    u8 pad_0x0[0x18];
    UnkActor* actor;
};

bool fn_710084f30c(UnkMissileGun_l84f30c* self, const CVector3D* pos, const CVector3D* dir,
                   UnkMissileOwner_l84f30c* owner, UnkCaster_l84f30c* caster) {
    f32 distance = 0.0f;
    bool found = false;
    fn_71008350e0(&g_unk_7172725ea8, self->actor, &distance, &found);
    f32 length = distance + 15.0f;
    CVector3D to = {pos->x - dir->x * length, pos->y - length * dir->y, pos->z - length * dir->z};
    CVector3D from = {pos->x, pos->y, pos->z};

    bool clear;
    if (found) {
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        if (tree == nullptr || caster->field_0x4a8 == nullptr || owner->field_0x2a0 == nullptr ||
            caster->field_0x4a8->shape == nullptr)
            return false;

        CVector2D a;
        fn_710007b430(&a, &from);
        CVector2D b;
        fn_710007b430(&b, &to);
        UnkSegment_l84f30c segment(a, b);
        UnkHitQuery_l84f30c query(a);
        if (!fn_7100d5c518(tree, 0x80401, reinterpret_cast<const UnkBox*>(&segment), &query, 2)) {
            clear = caster->IsClear(&to, &from, true);
        } else {
            CVector3D start = {query.point.x - dir->x, query.point.y - dir->y, query.point.z - dir->z};
            UnkCastShape_l84f30c* shape;
            if (caster->field_0x4a8 && (shape = caster->field_0x4a8->shape) && owner->field_0x2a0) {
                if (shape->field_0x60)
                    shape->vf_0x98();
                f32 half = (shape->field_0x48 - shape->field_0x40) * 0.5f;
                start.x = dir->x * half + start.x;
                start.y = dir->y * half + start.y;
                start.z = half * dir->z + start.z;
            }
            clear = caster->IsClear(&start, &from, true);
        }
    } else {
        clear = caster->IsClear(&to, &from, false);
    }

    if (clear) {
        if (UnkAnimationComponent* animation = owner->field_0x220) {
            const UnkIdTableSlot_l84f30c& ids = reinterpret_cast<const UnkIdTableSlot_l84f30c&>(GameManagerIdTable());
            return !fn_710117ae38(animation, ids.field_0x9c0.data->Key(), false);
        }
    }
    return false;
}
