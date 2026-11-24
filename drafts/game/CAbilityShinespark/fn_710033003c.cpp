// CAbilityShinespark's unit (tools/tu_map.py unit 433): the shinespark's contact damage. The owner's
// "ShinesparkActiveHits" collider, moved by the shinespark offset (+0x3ac), is checked for overlaps; every
// overlapped actor with a collision component gets a damage request pointing away from the owner, which its
// movement may refuse; an accepted one is scaled by the target's life or swarm component and sent to the
// target's damage receiver (+0x1b8). Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityShinespark.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU357.h"
#include "game/UnkTU54.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CCollider.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

struct UnkRefl_fn_710032df00;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710032df00>(void*);

extern const u64 g_unk_7101648e98;  // reached through the GOT

struct UnkSwarm_l33003c;
f32 fn_71007376d8(UnkSwarm_l33003c* swarm);

struct UnkShinesparkTunable_l33003c {
    u8 pad_0x0[0x38];
    f32 damage;  // 0x38
    u8 pad_0x3c[0x84 - 0x3c];
    f32 scale;           // 0x84
    f32 scale_grounded;  // 0x88
};

// A reference-counted handle released on destruction.
template <class T>
struct UnkHitRef_l33003c {
    T* ptr;
    ~UnkHitRef_l33003c() {
        if (T* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHitTarget_l33003c {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // 0x10
};

struct UnkHitHandle_l33003c : UnkRefObject {
    UnkHitTarget_l33003c* target;
};

#pragma pack(push, 4)
// One overlap the query found.
struct UnkHit_l33003c {
    UnkHitRef_l33003c<UnkHitHandle_l33003c> object;
    UnkHitRef_l33003c<UnkRefObject> field_0x8;
    s32 id;
    UnkVec3f point;  // 0x14
    u8 pad_0x20[0x28];
};
#pragma pack(pop)
static_assert(sizeof(UnkHit_l33003c) == 0x48, "hit");

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct alignas(8) UnkFixedList_l33003c {
    u32 count;
    u8 storage[N * sizeof(T)];

    UnkFixedList_l33003c() : count(0) {}
    ~UnkFixedList_l33003c() {
        for (u32 i = 0; i < count; ++i)
            begin()[i].~T();
    }
    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
};

// The overlap query the collision tree fills.
struct UnkHitQuery_l33003c {
    void** vtable;
    UnkFixedList_l33003c<UnkHit_l33003c, 8> hits;  // 0x8

    UnkHitQuery_l33003c() { vtable = g_unk_7101a23478 + 2; }
    ~UnkHitQuery_l33003c() { vtable = g_unk_7101a23478 + 2; }
};

struct UnkCollider_l33003c {
    u8 pad_0x0[0x158];
    UnkShape* shape;  // 0x158
};

struct UnkDamageRequest_l33003c {
    u8 pad_0x0[0x8];
    UnkWeakHandle* attacker;  // 0x8
    UnkWeakHandle* victim;    // 0x10
    u8 pad_0x18[0x48];
    UnkHitTarget_l33003c* hit;  // 0x60
    s32 field_0x68;
    UnkVec3f point;  // 0x6c
    f32 dir_x;       // 0x78
    f32 dir_y;
    f32 dir_z;
    u32 field_0x84[4];
    u32 field_0x94;
    UnkCollider_l33003c* collider;  // 0x98
    u8 pad_0xa0[0x14];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x40];
    f32 field_0xf8;
    u8 pad_0xfc[0x4];

    UnkDamageRequest_l33003c() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkDamageRequest_l33003c() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkDamageRequest_l33003c) == 0x100, "request layout");

struct UnkLife_l33003c : UnkReflObject {
    u8 pad_0x8[0xa0];
    u8 table[0x8];  // 0xa8
};

struct UnkDamageReceiverVtbl_l33003c {
    u8 pad_0x0[0x188];
    void (*vf_0x188)(void* self, UnkDamageRequest_l33003c* request);
};

struct UnkDamageReceiver_l33003c {
    UnkDamageReceiverVtbl_l33003c* vtbl;
    u8 pad_0x8[0x8];
    UnkReflObject* life;  // 0x10
    u8 pad_0x18[0x3c];
    f32 damage;  // 0x54
};

struct UnkVictim_l33003c {
    u8 pad_0x0[0x1b8];
    UnkDamageReceiver_l33003c* receiver;  // 0x1b8
};

struct UnkFilterVtbl_l33003c {
    u8 pad_0x0[0x78];
    s32 (*vf_0x78)(void* self, void* victim, void* attacker, UnkDamageRequest_l33003c* request);
};

struct UnkFilter_l33003c {
    UnkFilterVtbl_l33003c* vtbl;
};

struct UnkFilterHolder_l33003c {
    u8 pad_0x0[0x1610];
    UnkFilter_l33003c* filter;  // 0x1610
};

struct UnkMovementVtbl_l33003c {
    u8 pad_0x0[0x2a0];
    s32 (*vf_0x2a0)(void* self, void* victim, void* attacker);
};

struct UnkMovement_l33003c {
    UnkMovementVtbl_l33003c* vtbl;
};

struct UnkActor_l33003c {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x1a0];
    void* field_0x208;
    u8 pad_0x210[0x30];
    UnkFilterHolder_l33003c* field_0x240;
    u8 pad_0x248[0x58];
    UnkCollisionContainer* collision;  // 0x2a0

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};
static_assert(__builtin_offsetof(UnkActor_l33003c, collision) == 0x2a0, "layout");

struct UnkHitComponent_l33003c : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkActor_l33003c* actor;  // 0x30
};

struct UnkOwner_l33003c {
    u8 pad_0x0[0x30];
    UnkActor_l33003c* actor;
};

struct UnkShinespark_l33003c {
    u8 pad_0x0[0xc0];
    UnkOwner_l33003c* owner;
    u8 pad_0xc8[0x3ac - 0xc8];
    f32 offset_x;  // 0x3ac
    f32 offset_y;
    f32 offset_z;
};

static inline bool IsKindOf_l33003c(UnkReflObject* object, const base::reflection::CClass* cls) {
    return fn_710009d50c(object->vf_0x0(), cls);
}

static inline void* Target_l33003c(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

void fn_710033003c(UnkShinespark_l33003c* self) {
    UnkCollisionContainer* container = self->owner->actor->collision;
    UnkCollider_l33003c* collider;
    {
        CStrId name("ShinesparkActiveHits", true);
        collider = reinterpret_cast<UnkCollider_l33003c*>(fn_71008640a4(container, &name));
    }
    if (!collider)
        return;

    UnkGameManagerSub_0x608* collision = g_unk_7101d47310->field_0x608;
    UnkTree* tree = collision ? static_cast<UnkTree*>(collision->field_0x820) : nullptr;
    UnkHitQuery_l33003c query;

    const UnkShinesparkTunable_l33003c* tunable =
        reinterpret_cast<const UnkShinesparkTunable_l33003c*>(&g_unk_71726b0378);
    const f32& scale = self->owner->actor->field_0x208 ? tunable->scale : tunable->scale_grounded;
    f32 ox = scale * self->offset_x;
    f32 oy = scale * self->offset_y;
    f32 oz = scale * self->offset_z;
    UnkVec3f position;
    {
        UnkMatrix44 m = fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider));
        position.z = oz + m.m[11];
        position.x = ox + m.m[3];
        position.y = oy + m.m[7];
    }
    bool found;
    {
        UnkMatrix44_7100078c9c xf = BuildTranslationMatrixFromVec3(position);
        found = fn_7100d5b484(tree, 0x1080, collider->shape, reinterpret_cast<const UnkMtx44*>(&xf), &query, 0);
    }
    if (found) {
        UnkVec3f origin = self->owner->actor->Position();
        for (UnkHit_l33003c* hit = query.hits.begin(); hit != query.hits.end(); ++hit) {
            UnkHitHandle_l33003c* handle = hit->object.ptr;
            if (!handle || !handle->target)
                continue;
            UnkReflObject* component = handle->target->component;
            if (!component || !IsKindOf_l33003c(component, fn_710085fc44()))
                continue;
            UnkActor_l33003c* victim = static_cast<UnkHitComponent_l33003c*>(component)->actor;
            if (!victim)
                continue;

            f32 dx = hit->point.x - origin.x;
            f32 dy = hit->point.y - origin.y;
            f32 dz = hit->point.z - origin.z;
            UnkDamageRequest_l33003c request;
            f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
            request.field_0x68 = -1;
            request.point = hit->point;
            request.dir_x = -(dx * inv);
            request.dir_y = -(dy * inv);
            request.dir_z = -(dz * inv);
            request.field_0x84[0] = 0;
            request.field_0x84[1] = 0;
            request.field_0x84[2] = 0;
            request.field_0x84[3] = 0;
            request.field_0x94 = 0x28;
            request.hit = hit->object.ptr ? hit->object.ptr->target : nullptr;
            AssignRef(&request.attacker, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner->actor)));
            request.collider = collider;
            AssignRef(&request.victim, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(victim)));
            fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), tunable->damage);
            request.flags = 0x8000;

            if (UnkFilterHolder_l33003c* holder = victim->field_0x240) {
                UnkFilter_l33003c* filter = holder->filter;
                if (filter->vtbl->vf_0x78(filter, Target_l33003c(request.victim), Target_l33003c(request.attacker),
                                          &request) != 1)
                    continue;
            } else if (UnkMovement_l33003c* movement = reinterpret_cast<UnkMovement_l33003c*>(
                           base::reflection::refl_FindTyped<UnkRefl_fn_710032df00>(victim))) {
                if (movement->vtbl->vf_0x2a0(movement, Target_l33003c(request.victim), Target_l33003c(request.attacker)) !=
                    1)
                    continue;
            }

            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner->actor), &g_unk_7101648e98, false);
            UnkDamageReceiver_l33003c* receiver =
                reinterpret_cast<UnkVictim_l33003c*>(request.victim->target)->receiver;
            if (!receiver)
                continue;
            fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), receiver->damage);
            UnkReflObject* life = receiver->life;
            if (life && IsKindOf_l33003c(life, fn_71008b3b04())) {
                f32 factor = fn_71008c2de8(
                    reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(static_cast<UnkLife_l33003c*>(life)->table),
                    request.flags);
                if (!(factor >= 1.0f))
                    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), 1.0f / factor * request.field_0xf8 + 0.001f);
            } else if (UnkWeakTarget* swarm = fn_7100286ad4(Target_l33003c(request.victim))) {
                UnkReflObject* controller = reinterpret_cast<UnkHitTarget_l33003c*>(swarm)->component;
                if (controller && IsKindOf_l33003c(controller, fn_7100733464()))
                    fn_71008c1538(reinterpret_cast<UnkRequest*>(&request),
                                  fn_71007376d8(reinterpret_cast<UnkSwarm_l33003c*>(controller)));
            }
            receiver->vtbl->vf_0x188(receiver, &request);
        }
    }
}
