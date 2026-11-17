// CGooplotJumpAttack's unit (848): whether the way from the owner to its target is blocked. The owner's
// collider is swept toward the target (its position when it has one, else its colliders by name) and
// the first sweep that hits gives true; when none does, `dir` gets back its x and y with z cleared.
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726e7008;  // the owner's collider name

// A collider as the sweep uses it: slot 0x70 gives its world transform, its shape is at +0x158.
struct UnkGooplotCollider {
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
    virtual const UnkMatrix44& vf_0x70();
    u8 pad_0x8[0x150];
    UnkShape* shape;  // 0x158
};

struct UnkGooplotLayers {
    u8 pad_0x0[0x3c8];
    u32 mask;
};

struct UnkGooplotActor {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38: translation at m[3], m[7], m[11]
    u8 pad_0x78[0x210 - 0x78];
    void* field_0x210;
    UnkGooplotLayers* layers;  // 0x218
    u8 pad_0x220[0x2a0 - 0x220];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkGooplotJumpAttack {
    u8 pad_0x0[0x8];
    UnkGooplotActor* owner;
};

static inline UnkGooplotCollider* FindCollider(UnkCollisionContainer* collision, const CStrId* name) {
    return reinterpret_cast<UnkGooplotCollider*>(fn_71008640a4(collision, name));
}

static inline UnkGooplotActor* Target(UnkGooplotActor* actor) {
    return reinterpret_cast<UnkGooplotActor*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(actor)));
}

static inline CVector3D Position(const UnkGooplotActor* actor) {
    return CVector3D{actor->transform.m[3], actor->transform.m[7], actor->transform.m[11]};
}

static inline f32 Length(const CVector3D& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

static inline void Divide(CVector3D* v, f32 d) {
    v->x /= d;
    v->y /= d;
    v->z /= d;
}

// Sweeps `shape` along `dir` (flattened into `flat`) scaled by `distance` (into `delta`).
static inline bool Sweep(UnkTree* tree, u32 mask, UnkShape* shape, const UnkMatrix44& xf, const CVector3D* dir,
                         f32 distance, CVector2D* flat, CVector2D* delta) {
    fn_710007b430(flat, dir);
    delta->x = flat->x * distance;
    delta->y = flat->y * distance;
    return fn_7100d5babc(tree, mask, shape, reinterpret_cast<const UnkMtx44*>(&xf), reinterpret_cast<const Vec2*>(delta),
                         nullptr, false);
}

bool fn_710059462c(UnkGooplotJumpAttack* self, CVector3D* dir) {
    UnkGameManagerSub_0x608* world = g_unk_7101d47310->field_0x608;
    UnkTree* tree = world ? static_cast<UnkTree*>(world->field_0x820) : nullptr;

    UnkGooplotActor* owner = self->owner;
    const CVector3D from = Position(owner);
    UnkCollisionContainer* own_collision = owner->collision;
    UnkGooplotActor* target = Target(owner);
    UnkCollisionContainer* collision = target->collision;

    CStrId leg("RightLeg004", true);
    CStrId chest("Chest001", true);
    CVector2D delta;
    CVector2D flat;
    UnkGooplotCollider* own = FindCollider(own_collision, &g_unk_71726e7008);
    UnkGooplotCollider* chest_collider = FindCollider(collision, &chest);
    UnkGooplotCollider* body_collider = FindCollider(collision, &GameManagerIdTable().field_0x1650);
    UnkGooplotCollider* leg_collider = FindCollider(collision, &leg);
    u32 mask = self->owner->layers->mask;
    const CVector3D start = *dir;

    if (target->field_0x210) {
        dir->x = target->transform.m[3] - from.x;
        dir->y = target->transform.m[7] - from.y;
        dir->z = target->transform.m[11] - from.z;
        f32 distance = Length(*dir);
        dir->z = 0.0f;
        Divide(dir, distance);
        UnkShape* shape = own->shape;
        const UnkMatrix44& xf = fn_710002614c(&self->owner->transform);
        if (!Sweep(tree, mask, shape, xf, dir, distance - 16.0f, &flat, &delta))
            return true;

        dir->x = target->transform.m[3] - from.x;
        dir->y = target->transform.m[7] - from.y;
        dir->z = 0.0f;
        distance = Length(*dir);
        Divide(dir, distance);
        if (!Sweep(tree, mask, shape, xf, dir, distance, &flat, &delta))
            return true;
    } else {
        const UnkMatrix44& body = body_collider->vf_0x70();
        dir->z = 0.0f;
        dir->x = body.m[3] - from.x;
        dir->y = body.m[7] - from.y;
        f32 distance = Length(*dir);
        Divide(dir, distance);
        UnkShape* shape = own->shape;
        const UnkMatrix44& xf = fn_710002614c(&self->owner->transform);
        if (!Sweep(tree, mask, shape, xf, dir, distance - 16.0f, &flat, &delta))
            return true;

        const UnkMatrix44& chest_xf = chest_collider->vf_0x70();
        dir->x = chest_xf.m[3] - from.x;
        dir->y = chest_xf.m[7] - from.y;
        dir->z = 0.0f;
        distance = Length(*dir) - 16.0f;
        Divide(dir, distance);
        if (!Sweep(tree, mask, shape, xf, dir, distance, &flat, &delta))
            return true;

        const UnkMatrix44& leg_xf = leg_collider->vf_0x70();
        dir->x = leg_xf.m[3] - from.x;
        dir->y = leg_xf.m[7] - from.y;
        dir->z = 0.0f;
        distance = Length(*dir);
        Divide(dir, distance);
        if (!Sweep(tree, mask, shape, xf, dir, distance - 25.0f, &flat, &delta))
            return true;
    }
    *dir = CVector3D{start.x, start.y, 0.0f};
    return false;
}
