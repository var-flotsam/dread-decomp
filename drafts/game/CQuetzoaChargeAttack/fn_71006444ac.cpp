// CQuetzoaChargeAttack's unit (tools/tu_map.py unit 967): sweeps the owner's body from `pos` along the charge
// direction (scaled by the tunable's speed) through the collision tree, and the AI's own tree when it has one;
// on a hit puts the stop point in `out` and tells whether something solid is there. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/math/CVector.h"
#include "game/CCharacterMovement.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkTUProperty.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern void* g_unk_7101a23590[];
extern UnkTunable g_unk_71726faf68;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);
UnkTree* fn_710110c498(void* ai);

struct UnkLavaDropsShape_A2_b71;
void fn_7100d61750(UnkLavaDropsShape_A2_b71* self);

struct UnkChargeTunable_6444ac {
    void** vtable;
    u8 pad_0x8[0x1c];
    f32 speed;
};

struct UnkChargeFlagged_6444ac {
    u8 pad_0x0[0x3cc];
    u32 mask;
};

struct UnkChargeBody_6444ac {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

// What the hit object refers to: its flags come from a virtual call (0x10: solid).
struct UnkChargeCollider_6444ac {
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
    virtual u32 Flags();
};

struct UnkChargeHitObject_6444ac : base::UnkRefCounted {
    UnkChargeCollider_6444ac* collider;
};

struct UnkChargeHitRef_6444ac {
    UnkChargeHitObject_6444ac* ptr;

    UnkChargeHitRef_6444ac() : ptr(nullptr) {}
    ~UnkChargeHitRef_6444ac() {
        if (UnkChargeHitObject_6444ac* p = ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
    }
};

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the hit's vectors, how far
// along the sweep it is, and flags.
struct UnkChargeHit_6444ac {
    UnkChargeHitRef_6444ac object;
    UnkChargeHitRef_6444ac field_0x8;
    s32 id;
    u32 field_0x14[9];
    f32 fraction;
    u32 flags;
    u32 field_0x40;

    UnkChargeHit_6444ac() : id(-1), field_0x14(), fraction(0.0f), flags(0), field_0x40(0) {}
};

// The sweep's callback object: the closest hit so far. The vtables are the callback's and its base's.
struct UnkChargeHitQuery_6444ac {
    void** vtable;
    UnkChargeHit_6444ac hit;
    bool field_0x50;

    UnkChargeHitQuery_6444ac() {
        vtable = g_unk_7101a23590 + 2;
        field_0x50 = false;
    }
    ~UnkChargeHitQuery_6444ac() { vtable = g_unk_7101a20ed8 + 2; }
};

// A circle collision shape (0xb0 bytes).
struct UnkChargeCircle_6444ac {
    u8 raw[0xb0];

    explicit UnkChargeCircle_6444ac(f32 radius) {
        fn_7100d61664(this);
        fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(this), radius);
    }
    ~UnkChargeCircle_6444ac() { fn_7100d61750(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(this)); }
};

// One sweep: where the body is placed and the hit callback.
struct UnkChargeSweep_6444ac {
    UnkMatrix44_7100078c9c transform;
    UnkChargeHitQuery_6444ac query;
};

struct UnkQuetzoaChargeAttack_6444ac {
    void** vtable;
    UnkActor* actor;
};

static inline UnkTree* CollisionTree() {
    UnkGameManagerSub_0x608* sub = g_unk_7101d47310->field_0x608;
    return sub ? static_cast<UnkTree*>(sub->field_0x820) : nullptr;
}

// How far the charge moves: the direction's x and y scaled by the tunable's speed.
static inline Vec2 ChargeDelta(const CVector3D* dir) {
    f32 speed = reinterpret_cast<UnkChargeTunable_6444ac&>(g_unk_71726faf68).speed;
    CVector2D flat;
    fn_710007b430(&flat, dir);
    return Vec2(speed * flat.x, speed * flat.y);
}

bool fn_71006444ac(UnkQuetzoaChargeAttack_6444ac* self, const UnkVec3f* pos, const CVector3D* dir, UnkVec3f* out) {
    UnkActor* actor = self->actor;
    void* ai = actor->field_0x240;
    UnkFlagged* flagged = static_cast<UnkFlagged*>(actor->field_0x218);
    UnkChargeBody_6444ac* body =
        flagged ? reinterpret_cast<UnkChargeBody_6444ac*>(fn_7100976588(flagged)) : nullptr;
    UnkTree* tree = CollisionTree();
    if (!ai || !body || !tree)
        return false;

    u32 mask = reinterpret_cast<UnkChargeFlagged_6444ac*>(flagged)->mask;
    UnkChargeSweep_6444ac sweep;
    sweep.transform = fn_7100078cd8(*pos);
    Vec2 delta = ChargeDelta(dir);

    const UnkMtx44* transform = reinterpret_cast<const UnkMtx44*>(&sweep.transform);
    bool hit = fn_7100d5babc(tree, mask, body->shape, transform, &delta, &sweep.query, false);
    if (fn_710110c498(ai))
        hit = fn_7100d5babc(fn_710110c498(ai), mask, body->shape, transform, &delta, &sweep.query, false) | hit;
    if (!hit)
        return false;

    f32 t = sweep.query.hit.fraction;
    out->x = pos->x + delta.x * t;
    out->y = delta.y * t + pos->y;
    out->z = t * 0.0f + pos->z;

    UnkChargeHitObject_6444ac* object = sweep.query.hit.object.ptr;
    if (object && object->collider && (object->collider->Flags() & 0x10))
        return true;

    UnkChargeCircle_6444ac circle(150.0f);
    UnkMatrix44_7100078c9c moved = fn_7100078cd8(*out);
    for (int i = 0; i < 16; ++i)
        sweep.transform.m[i] = moved.m[i];
    return fn_7100d5b484(tree, 0x10, reinterpret_cast<UnkShape*>(&circle), transform, nullptr, 0);
}
