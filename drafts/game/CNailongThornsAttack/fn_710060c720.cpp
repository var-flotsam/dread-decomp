// CNailongThornsAttack's unit: spawns one thorn (the actor definition named at the tunables' +0x100) at the
// Nailong AI's current thorn node, facing along the node's x axis, sets up its movement component and aims it at
// the target, or at the wall between the owner and the thorn when the target is not reached. Names and
// layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoCommanderSentenceSphereAttack.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU365.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

// The unit's "damage" string id.
extern CStrId g_unk_71726f4d58 __attribute__((visibility("hidden")));

// A collision hit (0x48 bytes): the object hit and a second reference, an id, the point, two more vectors
// and flags.
struct UnkThornHit_l60c720 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    UnkVec3f point;
    UnkVec3f field_0x20;
    UnkVec3f normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkThornHit_l60c720()
        : object{nullptr}, field_0x8{nullptr}, id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f), field_0x38(0), flags(0), field_0x40(0) {}
};
static_assert(sizeof(UnkThornHit_l60c720) == 0x48, "UnkThornHit_l60c720 layout");

// The collision query's callback object: the closest hit so far and the query's origin.
struct UnkThornHitQuery_l60c720 {
    void** vtable;
    UnkThornHit_l60c720 hit;
    bool found;  // 0x50
    u32 field_0x54;
    CVector3D origin;
    u64 field_0x68;

    explicit UnkThornHitQuery_l60c720(const CVector3D& o) {
        vtable = g_unk_7101a20fa8 + 2;
        found = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkThornHitQuery_l60c720() { vtable = g_unk_7101a20ed8 + 2; }
};

// The segment fn_7100d5c518 tests: its two ends and the vector between them.
struct UnkThornSegment_l60c720 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkThornSegment_l60c720(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

struct alignas(8) UnkThornDefType_l60c720 {
    u32 id;
};

struct UnkThornAI_l60c720 : UnkReflObject {
    u8 pad_0x8[0x14840];
    CStrId* nodes;  // 0x14848
    u8 pad_0x14850[0x78];
    u32 field_0x148c8;
};

struct UnkThornsActor_l60c720 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x194];
    void* model;  // 0x228
    u8 pad_0x230[0x10];
    UnkReflObject* ai;  // 0x240
    u8 pad_0x248[0x130];
    u32 collision_mask;  // 0x378
};

struct UnkThornsTarget_l60c720 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x16c];
    UnkPlayerMovementCache* movement;  // 0x200
};

struct UnkThorn_l60c720 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38

    void vf_0xd0(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkThorn_l60c720*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
    void vf_0xd8(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkThorn_l60c720*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angle, flag);
    }
};

struct UnkThornCollider_l60c720 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkLavaDropsShape_A2_b71* shape;  // 0x158

    void* vf_0x70() { return reinterpret_cast<void* (*)(UnkThornCollider_l60c720*)>(vtable[0x70 / 8])(this); }
};

struct UnkThornMovement_l60c720 {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkReflObject* char_class;  // 0x10
    u8 pad_0x18[0x28];
    UnkVec3f dir;  // 0x40
    u8 pad_0x4c[0x590];
    u32 field_0x5dc;

    void vf_0x290(UnkThornsActor_l60c720* owner) {
        reinterpret_cast<void (*)(UnkThornMovement_l60c720*, UnkThornsActor_l60c720*)>(vtable[0x290 / 8])(this,
                                                                                                         owner);
    }
    void vf_0x2c0() { reinterpret_cast<void (*)(UnkThornMovement_l60c720*)>(vtable[0x2c0 / 8])(this); }
    void vf_0x368(bool flag) {
        reinterpret_cast<void (*)(UnkThornMovement_l60c720*, bool)>(vtable[0x368 / 8])(this, flag);
    }
    void vf_0x328(const UnkVec3f& point, const UnkVec3f& offset, const UnkThornHit_l60c720& hit,
                  UnkThornCollider_l60c720* collider) {
        reinterpret_cast<void (*)(UnkThornMovement_l60c720*, const UnkVec3f*, const UnkVec3f*,
                                  const UnkThornHit_l60c720*, UnkThornCollider_l60c720*)>(vtable[0x328 / 8])(
            this, &point, &offset, &hit, collider);
    }
    void vf_0x330(UnkActor* target, const UnkVec3f& pos, const UnkVec3f& offset, const UnkThornHit_l60c720& hit,
                  UnkThornCollider_l60c720* collider) {
        reinterpret_cast<void (*)(UnkThornMovement_l60c720*, UnkActor*, const UnkVec3f*, const UnkVec3f*,
                                  const UnkThornHit_l60c720*, UnkThornCollider_l60c720*)>(vtable[0x330 / 8])(
            this, target, &pos, &offset, &hit, collider);
    }
};

struct UnkThornsTunables_l60c720 {
    u8 pad_0x0[0x100];
    CStrId thorn;  // 0x100
    u8 pad_0x108[0x14];
    bool field_0x11c;
};

struct UnkThornsAttack_l60c720 {
    u8 pad_0x0[0x8];
    UnkThornsActor_l60c720* owner;  // 0x8
    u8 pad_0x10[0x18];
    UnkThornsTunables_l60c720* tunables;  // 0x28
    u8 pad_0x30[0x58];
    u32 node;  // 0x88

    UnkThornsTarget_l60c720* Target() {
        return reinterpret_cast<UnkThornsTarget_l60c720*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(owner)));
    }
};

UnkThornMovement_l60c720* fn_710060d778(UnkThorn_l60c720* thorn);

static inline bool Differs(f32 a, f32 b) {
    return a - 0.001f > b || a + 0.001f < b;
}

void fn_710060c720(UnkThornsAttack_l60c720* self) {
    UnkReflObject* ai_object = self->owner->ai;
    UnkThornAI_l60c720* ai = ai_object && fn_710009d50c(ai_object->vf_0x0(), fn_710061f464())
                                 ? static_cast<UnkThornAI_l60c720*>(ai_object)
                                 : nullptr;
    u32 node = self->node;
    u64 key = ai->nodes[node].data->Key();
    UnkMatrix44 m;
    fn_71011735e8(static_cast<UnkEntity*>(self->owner->model), &key, &m, 0);
    UnkVec3f dir(m.m[0], m.m[4], m.m[8]);
    UnkVec3f pos(0.0f, 0.0f, 0.0f);
    fn_710117364c(self->owner->model, &key, reinterpret_cast<UnkGrappleAnswer_b50*>(&pos));

    UnkThornDefType_l60c720 type;
    UnkThorn_l60c720* thorn;
    {
        UnkActorDefFactory* factory = g_unk_71726a0220;
        CStrId name = self->tunables->thorn;
        ++name.data->reads;
        const char* path = reinterpret_cast<const char*>(&name.data->String());
        CStrId id = self->tunables->thorn;
        type.id = g_unk_7172a0ba48;
        thorn = reinterpret_cast<UnkThorn_l60c720*>(
            fn_710119438c(factory, path, &id,
                          reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    }
    if (!thorn)
        return;
    UnkVec3f angle = ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(dir);
    thorn->vf_0xd0(&pos, true);
    thorn->vf_0xd8(&angle, true);

    UnkThornMovement_l60c720* movement = fn_710060d778(thorn);
    if (!movement || !movement->char_class ||
        !fn_710009d50c(movement->char_class->vf_0x0(), fn_71006227bc()))
        return;
    movement->field_0x5dc = ai->field_0x148c8;
    movement->dir = dir;
    movement->vf_0x290(self->owner);
    movement->vf_0x2c0();
    movement->vf_0x368(self->tunables->field_0x11c);

    void* body = fn_71010d7a6c(self->Target()->movement);
    auto* collision = static_cast<UnkCollisionContainer*>(fn_710043acb4(thorn));
    if (!collision)
        return;
    auto* collider = reinterpret_cast<UnkThornCollider_l60c720*>(fn_71008640a4(collision, &g_unk_71726f4d58));
    if (!body || !collider)
        return;

    UnkVec3f thorn_pos = thorn->xform.GetTranslation();
    UnkThornsTarget_l60c720* target = self->Target();
    UnkThornHitQuery_l60c720 query(CVector3D{target->xform.m[0][3], target->xform.m[1][3], target->xform.m[2][3]});
    fn_7100d57ca0(collider->shape, collider->vf_0x70(), body, &query, 0);
    if (query.found) {
        UnkActor* aim = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
        UnkThornsTarget_l60c720* t = self->Target();
        movement->vf_0x330(aim, UnkVec3f(t->xform.m[0][3], t->xform.m[1][3], t->xform.m[2][3]),
                           UnkVec3f(0.0f, 0.0f, 0.0f), query.hit, collider);
    } else {
        UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
        UnkThornsActor_l60c720* owner = self->owner;
        f32 ox = owner->xform.m[0][3];
        f32 oy = owner->xform.m[1][3];
        UnkThornSegment_l60c720 segment(CVector2D{ox, oy}, CVector2D{thorn_pos.x, thorn_pos.y});
        UnkThornHitQuery_l60c720 wall(CVector3D{ox, oy, owner->xform.m[2][3]});
        fn_7100d5c518(tree, owner->collision_mask, reinterpret_cast<const UnkBox*>(&segment), &wall, 2);
        if (wall.found && (Differs(wall.hit.point.x, thorn_pos.x) || Differs(wall.hit.point.y, thorn_pos.y) ||
                           Differs(wall.hit.point.z, thorn_pos.z)))
            movement->vf_0x328(wall.hit.point, UnkVec3f(0.0f, 0.0f, 0.0f), wall.hit, collider);
    }
}
