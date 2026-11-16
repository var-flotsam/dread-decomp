// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): looks up the owner's damage-shapes track
// and, for the first of its shape objects that touches the target's body, sends the target a damage request
// named by the track. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCharClassAttackComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkRequest;
struct UnkStruct_71008c0c74;
struct UnkBase_71003cc474;

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

struct UnkLavaDropsShape_A2_b71;
bool fn_7100d57ca0(UnkLavaDropsShape_A2_b71* self, void* body, void* other, void* out, u32 flags);

struct UnkShapesDamageRequest_l4d29dc {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* target;  // 0x10
    u8 pad_0x18[0x12];
    bool field_0x2a;
    u8 pad_0x2b[0x3d];
    s32 field_0x68;
    f32 point_x;  // 0x6c
    f32 point_y;
    f32 point_z;
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u32 field_0x90;
    u32 field_0x94;
    u8 pad_0x98[0x1c];
    u32 flags;  // 0xb4
    u8 id_raw[0x8];  // 0xb8: a CStrId the constructor sets
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
    u8 pad_0xcc[0x34];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkShapesDamageRequest_l4d29dc() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkShapesDamageRequest_l4d29dc() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkShapesDamageRequest_l4d29dc) == 0x100, "request layout");

struct UnkShapesReceiver_l4d29dc {
    void** vtable;
    void vf_0x190(UnkActor* actor, UnkShapesDamageRequest_l4d29dc* request, void* a, void* b) {
        reinterpret_cast<void (*)(UnkShapesReceiver_l4d29dc*, UnkActor*, UnkShapesDamageRequest_l4d29dc*, void*,
                                  void*)>(vtable[0x190 / 8])(this, actor, request, a, b);
    }
};

struct UnkShapeKey_l4d29dc {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkShapesTrack_l4d29dc {
    u8 pad_0x0[0x20];
    UnkShapeKey_l4d29dc** shapes;  // 0x20
    u32 count;                     // 0x28
    u8 pad_0x2c[0x1c];
    CStrId name;  // 0x48
    u32 field_0x50;
};

struct UnkShape_l4d29dc {
    void** vtable;
    u8 body[0x8];  // 0x8

    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkShape_l4d29dc*)>(vtable[0x18 / 8])(this); }
};

struct UnkShapeHolder_l4d29dc {
    u8 pad_0x0[0x30];
    UnkShape_l4d29dc* shape;
};

struct UnkCooldownOwner_l4d29dc {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;  // 0x200
    u8 pad_0x208[0x18];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x30];
    UnkShapesReceiver_l4d29dc* receiver;  // 0x258
};

struct UnkCooldownAI_l4d29dc {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
};

static UnkCooldownOwner_l4d29dc* AsOwner(UnkActor* actor) {
    return reinterpret_cast<UnkCooldownOwner_l4d29dc*>(actor);
}

// The track a query found: the damage-shapes track.
inline UnkShapesTrack_l4d29dc* Track(const UnkTrackQuery& query) {
    return reinterpret_cast<UnkShapesTrack_l4d29dc*>(query.result);
}

// The game manager's object with this id, of the main-camera kind.
inline void* FindObject(u64 id) {
    alignas(8) u32 kind = g_unk_71727bebb8;
    return fn_7100deba8c(g_unk_7101d47310, &id, &kind);
}

void fn_71004d29dc(UnkCooldownAI_l4d29dc* self) {
    UnkCooldownOwner_l4d29dc* owner = AsOwner(self->owner);
    UnkAnimationComponent* animation = owner->animation;
    UnkShapesReceiver_l4d29dc* receiver = owner->receiver;
    if (!animation || !receiver)
        return;

    UnkTrackQuery query;
    query.cls = fn_7100c12238();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(animation, &query);
    if (!query.result)
        return;

    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return;
    UnkPlayerMovementCache* movement = AsOwner(fn_7100d7c990(self->owner))->movement;
    if (!movement)
        return;
    void* body = fn_71010d7a6c(movement);
    if (!body)
        return;

    for (UnkShapeKey_l4d29dc** it = Track(query)->shapes, **end = it + Track(query)->count; it != end; ++it) {
        void* found = FindObject((*it)->id);
        if (!found)
            continue;
        UnkShapeHolder_l4d29dc* holder = reinterpret_cast<UnkShapeHolder_l4d29dc*>(fn_710028aca8(found));
        if (!holder)
            continue;
        UnkShape_l4d29dc* shape = holder->shape;
        if (!shape || shape->vf_0x18() != 0x10)
            continue;
        if (!fn_7100d57ca0(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(shape), shape->body, body, nullptr, 0))
            continue;
        if (Track(query)->name.data == &g_unk_7101d47108->empty)
            continue;

        UnkShapesDamageRequest_l4d29dc request;
        request.Id() = Track(query)->name;
        fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), fn_7100789f5c(&Track(query)->name));
        request.field_0xc8 = Track(query)->field_0x50;
        AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
        AssignRef(&request.target, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(target)));
        request.point_x = target->field_0x44;
        request.point_y = target->field_0x54;
        request.point_z = target->field_0x64;
        request.field_0x68 = -1;
        request.field_0x78 = 0;
        request.field_0x80 = 0;
        request.field_0x88 = 0;
        request.field_0x90 = 0;
        request.field_0x94 = 8;
        request.field_0x2a = false;
        request.flags = 0x4000000;
        receiver->vf_0x190(target, &request, nullptr, nullptr);
        return;
    }
}
