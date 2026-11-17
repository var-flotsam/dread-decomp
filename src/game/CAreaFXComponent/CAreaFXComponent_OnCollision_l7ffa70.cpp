// CAreaFXComponent's unit: handles a collision message (type 0x10000). When the colliding object's
// collision id is g_unk_7172721530 and it is flagged (+0x6a), sends the owner's damage target (+0x1b8)
// a damage request built from the message, then refreshes the owner's listed collision objects.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCollisionComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;
struct UnkEntry;

extern CStrId g_unk_7172721530 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172721538[] __attribute__((visibility("hidden")));

// A vector copied a component at a time.
struct UnkVec3_l7ffa70 {
    f32 x;
    f32 y;
    f32 z;

    UnkVec3_l7ffa70& operator=(const UnkVec3_l7ffa70& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkCollider_l7ffa70 {
    u8 pad_0x0[0x6a];
    bool field_0x6a;
};

struct UnkColliderRef_l7ffa70 {
    u8 pad_0x0[0x10];
    UnkCollider_l7ffa70* collider;
};

struct UnkComponent_l7ffa70 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x28];
    void* owner;  // 0x30
};

struct UnkSource_l7ffa70 {
    u8 pad_0x0[0x10];
    UnkComponent_l7ffa70* component;
};

struct UnkSourceRef_l7ffa70 {
    u8 pad_0x0[0x10];
    UnkSource_l7ffa70* source;
};

struct UnkCollisionMsg_l7ffa70 {
    u8 pad_0x0[0x10];
    UnkSourceRef_l7ffa70* source;      // 0x10
    UnkColliderRef_l7ffa70* collider;  // 0x18
    UnkVec3_l7ffa70 points[4];         // 0x20
    u8 pad_0x50[0x8];
    u32 type;  // 0x58
};

struct UnkDamageRequest_l7ffa70 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* owner;   // 0x10
    u8 pad_0x18[0x48];
    UnkCollider_l7ffa70* collider;  // 0x60
    UnkVec3_l7ffa70 points[4];      // 0x68
    UnkSource_l7ffa70* hit_source;  // 0x98
    u8 pad_0xa0[0x14];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x48];

    UnkDamageRequest_l7ffa70() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkDamageRequest_l7ffa70() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkDamageRequest_l7ffa70) == 0x100, "request layout");

struct UnkDamageTarget_l7ffa70 {
    u8 pad_0x0[0x50];
    f32 damage;
};
void fn_7101160e70(UnkDamageTarget_l7ffa70* self, UnkDamageRequest_l7ffa70* request);

struct UnkAreaEntity_l7ffa70 {
    u8 pad_0x0[0x1b8];
    UnkDamageTarget_l7ffa70* damage_target;  // 0x1b8
    u8 pad_0x1c0[0xe0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkAreaOwner_l7ffa70 {
    u8 pad_0x0[0x30];
    UnkAreaEntity_l7ffa70* entity;
    u8 pad_0x38[0x3c4 - 0x38];
    s32 object_count;  // 0x3c4
};

struct UnkAreaFX_l7ffa70 {
    u8 pad_0x0[0x10];
    UnkAreaOwner_l7ffa70* owner;
};

bool fn_71007ffa70(UnkAreaFX_l7ffa70* self, UnkCollisionMsg_l7ffa70* msg) {
    if (msg->type != 0x10000)
        return false;
    UnkCollider_l7ffa70* collider = msg->collider ? msg->collider->collider : nullptr;
    UnkCollisionContainer* collision = self->owner->entity->collision;
    CStrId* id = fn_7100864018(collision, collider);
    if (!collider->field_0x6a || !(*id == g_unk_7172721530))
        return false;

    UnkDamageTarget_l7ffa70* target = self->owner->entity->damage_target;
    if (!target)
        return true;
    UnkSource_l7ffa70* source = msg->source ? msg->source->source : nullptr;
    UnkDamageRequest_l7ffa70 request;
    request.points[0] = msg->points[0];
    request.points[1] = msg->points[1];
    request.points[2] = msg->points[2];
    request.points[3] = msg->points[3];
    request.collider = collider;
    request.hit_source = source;

    UnkComponent_l7ffa70* component = source->component;
    UnkComponent_l7ffa70* hit = fn_710009d50c(component->vf_0x0(), fn_710085fc44()) ? component : nullptr;
    AssignRef(&request.source, fn_7100d78378(static_cast<UnkWeakTarget*>(hit->owner)));
    AssignRef(&request.owner, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner->entity)));
    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), target->damage);
    request.flags = 0x800;
    fn_7101160e70(target, &request);

    for (s32 i = 0; i < self->owner->object_count; ++i)
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collision, &g_unk_7172721538[i])), false);
    return true;
}
