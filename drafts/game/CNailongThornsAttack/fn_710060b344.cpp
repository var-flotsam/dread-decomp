// CNailongThornsAttack's unit: picks the Nailong AI's thorn node to attack from. None while the owner is frozen
// or (when not aiming at the AI's point) while the game manager says so. Aiming, it is the node whose direction
// best follows the way to the aimed point; otherwise the first node facing the target whose sweep from the
// owner reaches the target's body. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CFrozenComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

#include <algorithm>

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag);

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

// What a hit's object holds: the bodies it belongs to.
struct UnkThornBodies_l60b344 {
    u8 pad_0x0[0x164];
    u32 count;
    void* items[1];  // 0x168

    bool Contains(void* body) const {
        if (count == 0)
            return false;
        return std::find(items, items + count, body) != items + count;
    }
};

struct UnkThornHitObj_l60b344 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
    UnkThornBodies_l60b344* bodies;  // 0x10
};

struct UnkThornHitRef_l60b344 {
    UnkThornHitObj_l60b344* ptr;

    UnkThornHitRef_l60b344() : ptr(nullptr) {}
    ~UnkThornHitRef_l60b344() {
        if (UnkThornHitObj_l60b344* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkThornHit_l60b344 {
    UnkThornHitRef_l60b344 object;
    UnkThornHitRef_l60b344 field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkThornHit_l60b344()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkThornHit_l60b344) == 0x48, "UnkThornHit_l60b344 layout");

// The sweep's callback object: the closest hit so far and the sweep's origin.
struct UnkThornSweepQuery_l60b344 {
    void** vtable;
    UnkThornHit_l60b344 hit;
    bool found;  // 0x50
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkThornSweepQuery_l60b344(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        found = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkThornSweepQuery_l60b344() { vtable = g_unk_7101a20ed8 + 2; }
};

// The swept circle.
struct UnkThornCircle_l60b344 {
    u8 raw[0xb0];

    UnkThornCircle_l60b344() { fn_7100d61664(this); }
    ~UnkThornCircle_l60b344() { fn_7100d61750(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(this)); }
    void SetRadius(f32 radius) { fn_7100d618f8(reinterpret_cast<UnkSphereShape_ovb0135*>(this), radius); }
};

// A point on the plane: the x and y of a position.
struct UnkThornProbe_l60b344 : CVector2D {
    explicit UnkThornProbe_l60b344(const CVector3D& v) { fn_710007b430(this, &v); }
};

// A thorn node: its id, its position relative to the owner and its direction on the plane.
struct UnkThornNode_l60b344 {
    CStrId id;
    CVector2D offset;  // 0x8
    CVector2D dir;     // 0x10
};

struct UnkThornAI_l60b344 : UnkReflObject {
    u8 pad_0x8[0x14691];
    bool aiming;  // 0x14699
    u8 pad_0x1469a[0x2];
    UnkVec3f aim;  // 0x1469c
    u8 pad_0x146a8[0x1c8];
    UnkThornNode_l60b344* nodes;  // 0x14870
    u32 node_count;               // 0x14878
    u8 pad_0x1487c[0x4c];
    u32 mode;  // 0x148c8
};
static_assert(__builtin_offsetof(UnkThornAI_l60b344, nodes) == 0x14870, "UnkThornAI_l60b344 layout");
static_assert(__builtin_offsetof(UnkThornAI_l60b344, mode) == 0x148c8, "UnkThornAI_l60b344 layout");

struct UnkThornsTarget_l60b344 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x16c];
    UnkPlayerMovementCache* movement;  // 0x200
};

struct UnkThornsOwner_l60b344 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x1ac];
    UnkThornAI_l60b344* ai;  // 0x240
    u8 pad_0x248[0x80];
    void* frozen;  // 0x2c8
};

struct UnkThornsAttack_l60b344 {
    u8 pad_0x0[0x8];
    UnkThornsOwner_l60b344* owner;  // 0x8

    UnkThornsTarget_l60b344* Target() {
        return reinterpret_cast<UnkThornsTarget_l60b344*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(owner)));
    }
};

struct UnkThornsAttack_b68a4;

CStrId fn_710060b344(UnkThornsAttack_b68a4* attack) {
    auto* self = reinterpret_cast<UnkThornsAttack_l60b344*>(attack);
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (self->owner->frozen && fn_71007f9f20(self->owner->frozen))
        return g_unk_7101d470e0;
    bool aiming = self->owner->ai && self->owner->ai->aiming;
    bool paused = fn_7100df5740(g_unk_7101d47310);
    if (!aiming && paused)
        return g_unk_7101d470e0;

    void* body = fn_71010d7a6c(self->Target()->movement);
    UnkThornAI_l60b344* ai_object = self->owner->ai;
    UnkThornAI_l60b344* ai =
        ai_object && fn_710009d50c(ai_object->vf_0x0(), fn_710061f464()) ? ai_object : nullptr;
    UnkThornsTarget_l60b344* target = self->Target();
    UnkThornsOwner_l60b344* owner = self->owner;
    f32 dx = (aiming ? ai->aim.x : target->xform.m[0][3]) - owner->xform.m[0][3];
    f32 dy = (aiming ? ai->aim.y : target->xform.m[1][3]) - owner->xform.m[1][3];
    f32 dz = (aiming ? ai->aim.z : target->xform.m[2][3]) - owner->xform.m[2][3];
    f32 length = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);

    CStrId best = g_unk_7101d470e0;
    f32 best_dot = 0.0f;
    UnkThornNode_l60b344* nodes = ai->nodes;
    UnkThornNode_l60b344* end = nodes + ai->node_count;
    if (nodes != end) {
        f32 inv = 1.0f / (length + 0.0001f);
        f32 nx = dx * inv;
        f32 ny = dy * inv;
        f32 nz = dz * inv;
        for (UnkThornNode_l60b344* node = nodes; node != end; ++node) {
            f32 ex = node->dir.x;
            f32 ey = node->dir.y;
            f32 dir_length = __builtin_sqrtf(ex * ex + ey * ey);
            if (dir_length != 0.0f) {
                ex /= dir_length;
                ey /= dir_length;
            }
            f32 dot = nx * ex + ny * ey + nz * 0.0f;
            if (dot < 0.8f)
                continue;
            if (aiming) {
                if (dot > best_dot) {
                    best = node->id;
                    best_dot = dot;
                }
                continue;
            }
            CVector2D from = UnkThornProbe_l60b344(CVector3D{self->owner->xform.m[0][3], self->owner->xform.m[1][3],
                                                             self->owner->xform.m[2][3]});
            if (!body)
                continue;
            f32 x = from.x + node->offset.x;
            f32 y = from.y + node->offset.y;
            f32 radius = ai->mode == 2 ? 20.0f : 10.0f;
            UnkThornCircle_l60b344 circle;
            circle.SetRadius(radius);
            CVector2D sweep;
            sweep.x = (ex * 2500.0f + x) - x;
            sweep.y = (ey * 2500.0f + y) - y;
            UnkMatrix44 m = {};
            m.m[15] = 1.0f;
            m.m[10] = 1.0f;
            m.m[5] = 1.0f;
            m.m[0] = 1.0f;
            m.m[3] = x;
            m.m[7] = y;
            UnkThornSweepQuery_l60b344 query(CVector3D{x, y, 0.0f});
            if (fn_7100d5babc(tree, 0x80429, reinterpret_cast<UnkShape*>(&circle),
                              reinterpret_cast<const UnkMtx44*>(&m), reinterpret_cast<const Vec2*>(&sweep), &query,
                              false) &&
                query.hit.object.ptr->bodies->Contains(body))
                return node->id;
        }
    }
    if (aiming)
        return best;
    return g_unk_7101d470e0;
}
