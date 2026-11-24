// CAbilityShinespark's unit (tools/tu_map.py unit 433): moves `from` by what the owner actor's movement (+0x1f8)
// resolves for its collider; when that moves the point, sweeps the collider along -50 times the offset through
// the game manager's collision tree and keeps the swept point. Writes the point to `out` and returns whether the
// movement resolved. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

extern void* g_unk_7101a23590[];

struct UnkHitObj_l32bd6c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkHitRef_l32bd6c {
    UnkHitObj_l32bd6c* ptr;

    UnkHitRef_l32bd6c() : ptr(nullptr) {}
    ~UnkHitRef_l32bd6c() {
        if (UnkHitObj_l32bd6c* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitVec_l32bd6c {
    f32 x, y, z;
    UnkHitVec_l32bd6c(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l32bd6c {
    UnkHitRef_l32bd6c object;
    UnkHitRef_l32bd6c field_0x8;
    s32 id;
    UnkHitVec_l32bd6c point;
    UnkHitVec_l32bd6c field_0x20;
    UnkHitVec_l32bd6c normal;
    f32 fraction;  // 0x38: how far along the sweep the hit is
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l32bd6c()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), fraction(0),
          flags(0), field_0x40(0) {}
};

// The hit query the sweep fills.
struct UnkSweepQuery_l32bd6c {
    void** vtable;
    UnkCollisionHit_l32bd6c hit;
    bool field_0x50;

    UnkSweepQuery_l32bd6c() {
        field_0x50 = false;
        vtable = g_unk_7101a23590 + 2;
    }
    ~UnkSweepQuery_l32bd6c() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkCollider_l32bd6c {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

struct UnkMovement_l32bd6c;

struct UnkMovementVtbl_l32bd6c {
    u8 pad_0x0[0x200];
    bool (*vf_0x200)(UnkMovement_l32bd6c* self, const UnkVec3f* from, UnkVec3f* offset, u32 mask,
                     UnkShape* shape, s32 flags);
    u8 pad_0x208[0x2a8 - 0x208];
    UnkCollider_l32bd6c* (*vf_0x2a8)(UnkMovement_l32bd6c* self);  // the collider
};

struct UnkMovement_l32bd6c {
    UnkMovementVtbl_l32bd6c* vtbl;
    u8 pad_0x8[0x3cc - 0x8];
    u32 mask;
};

struct UnkActor_l32bd6c {
    u8 pad_0x0[0x1f8];
    UnkMovement_l32bd6c* movement;
};

struct UnkOwner_l32bd6c {
    u8 pad_0x0[0x30];
    UnkActor_l32bd6c* actor;
};

struct UnkShinespark_l32bd6c {
    u8 pad_0x0[0xc0];
    UnkOwner_l32bd6c* owner;
};

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

bool fn_710032bd6c(UnkShinespark_l32bd6c* self, const UnkVec3f* from, UnkVec3f* out) {
    UnkVec3f point = *from;
    UnkMovement_l32bd6c* movement = self->owner->actor->movement;
    UnkCollider_l32bd6c* collider = movement->vtbl->vf_0x2a8(movement);
    UnkVec3f offset(0.0f, 0.0f, 0.0f);
    u32 movementMask = movement->mask;
    UnkShape* movementShape = collider->shape;
    bool resolved = movement->vtbl->vf_0x200(movement, &point, &offset, movementMask, movementShape, 8);
    if (!resolved)
        return resolved;
    if (!(IsNearZero(offset.x) && IsNearZero(offset.y) && IsNearZero(offset.z))) {
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        UnkSweepQuery_l32bd6c query;
        f32 x = offset.x * -50.0f;
        f32 y = offset.y * -50.0f;
        f32 z = offset.z * -50.0f;
        base::math::CVector3D sweep = {x, y, z};
        u32 mask = movement->mask;
        UnkShape* shape = collider->shape;
        UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(point);
        base::math::CVector2D sweep2;
        fn_710007b430(&sweep2, &sweep);
        if (fn_7100d5babc(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf),
                          reinterpret_cast<const Vec2*>(&sweep2), &query, false)) {
            f32 t = query.hit.fraction;
            point.x = t * sweep.x + point.x;
            point.y = t * sweep.y + point.y;
            point.z = t * sweep.z + point.z;
        }
    }
    *out = point;
    return resolved;
}
