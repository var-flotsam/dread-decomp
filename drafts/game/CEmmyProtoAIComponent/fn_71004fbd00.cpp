// CEmmyProtoAIComponent unit (tools/tu_map.py unit 791): the per-frame step of the prototype EMMI's grab
// tutorial. Marks the tutorial launched once the player is ahead, starts the slow-motion slide (with its
// sound) when the player is close, ends the tutorial when the player has got past, and counts down the
// push cooldown. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkTU39.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXformNode.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::UnkRefPtrMember;
using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStrIdInline;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c2f0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);
void* fn_7100df6d84(UnkGameManager* gm);

// The property as a blackboard definition (game/UnkGameBlackboard.h): its qualified id at +0x10.
struct UnkProtoNamedId_A3_b66 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedId qid;
};

struct UnkEmmyProtoAI_l4fbd00;
void fn_710054d16c(UnkEmmyProtoAI_l4fbd00* self, f32 dt);
void fn_71004fb4ec(UnkEmmyProtoAI_l4fbd00* self, bool slow);
void fn_71004fd510(void* handle, void* slot);
void fn_71004fd8cc();
bool fn_7100864e34(void* collision, u32 mask, void* shape, void* transform, void* query, bool flag);

// The tunables: the distance the player must get past the EMMI by.
struct UnkEmmyProtoTunables_l4fbd00 {
    u8 pad_0x0[0x228];
    f32 pass_distance;  // 0x228
};
extern UnkEmmyProtoTunables_l4fbd00* g_unk_71726dcc88;  // reached through the GOT

// The game manager's slow-motion callback.
struct UnkSlomoManager_l4fbd00 {
    u8 pad_0x0[0x720];
    void* callback_owner;  // 0x720
    void (*callback)();    // 0x728
};

struct UnkEmmyProtoHit_l4fbd00 {
    void** vtable;
    u8 pad_0x8[0x150];
    void* shape;  // 0x158

    void* Transform() { return reinterpret_cast<void* (*)(UnkEmmyProtoHit_l4fbd00*)>(vtable[0x70 / 8])(this); }
};

struct UnkEmmyProtoPart_l4fbd00 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
    u8 pad_0x14[0x1e4];
    u8 timer[8];  // 0x1f8
};

struct UnkEmmyProtoParts_l4fbd00 {
    void** vtable;
    u8 pad_0x8[0x2d10];
    u32 count;  // 0x2d18
    UnkEmmyProtoPart_l4fbd00* parts[1] __attribute__((packed));  // 0x2d1c

    UnkEmmyProtoHit_l4fbd00* Hit() {
        return reinterpret_cast<UnkEmmyProtoHit_l4fbd00* (*)(UnkEmmyProtoParts_l4fbd00*)>(vtable[0x2a8 / 8])(this);
    }
};

struct UnkEmmyProtoMove_l4fbd00 {
    u8 pad_0x0[0x70];
    f32 dir[3];  // 0x70
};

struct UnkEmmyProtoActor_l4fbd00 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x1a0];
    UnkEmmyProtoParts_l4fbd00* parts;  // 0x208
    u8 pad_0x210[0x8];
    UnkEmmyProtoMove_l4fbd00* move;  // 0x218
    UnkActorTarget* animation;  // 0x220
    u8 pad_0x228[0x70];
    void* pushable;  // 0x298
    void* collision;  // 0x2a0

    f32 X() const { return transform[0][3]; }
    UnkVec3f Position() const { return UnkVec3f(transform[0][3], transform[1][3], transform[2][3]); }
};
static_assert(__builtin_offsetof(UnkEmmyProtoActor_l4fbd00, parts) == 0x208, "layout");
static_assert(__builtin_offsetof(UnkEmmyProtoActor_l4fbd00, collision) == 0x2a0, "layout");

static inline UnkEmmyProtoActor_l4fbd00* TargetOf(UnkEmmyProtoActor_l4fbd00* actor) {
    return reinterpret_cast<UnkEmmyProtoActor_l4fbd00*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(actor)));
}

// The collision query the hit test fills: two references, an id and the hit's data.
struct UnkEmmyProtoQuery_l4fbd00 {
    void** vtable;
    UnkRefPtrMember object;
    UnkRefPtrMember other;
    s32 id;
    u8 field_0x1c[0x38];

    UnkEmmyProtoQuery_l4fbd00() : object{nullptr}, other{nullptr}, id(-1), field_0x1c() {
        vtable = g_unk_7101a20ed8 + 2;
    }
    ~UnkEmmyProtoQuery_l4fbd00() { vtable = g_unk_7101a20ed8 + 2; }
};

// The kind-5 push block: the push direction and its flags.
struct UnkEmmyProtoPush_l4fbd00 {
    void** vtable;
    f32 dir[3];  // 0x8
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    u8 pad_0x38;
    u8 field_0x39;
    u8 field_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

struct UnkEmmyProtoState_l4fbd00 {
    bool active;  // 0x29f26
    u8 pad_0x1[0x7e1];
    bool sliding;  // 0x2a708
    bool tutorial;  // 0x2a709
    u8 pad_0x7e4;
    bool slomo;  // 0x2a70b
};

struct UnkEmmyProtoGrab_l4fbd00 {
    void* handle;  // 0x2a710
    u8 pad_0x8[0x10];
    f32 cooldown;  // 0x2a728
};

struct UnkEmmyProtoAI_l4fbd00 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEmmyProtoActor_l4fbd00* owner;  // 0x30
    u8 pad_0x38[0x29418 - 0x38];
    u8 slide_timer[0x29f26 - 0x29418];  // 0x29418
    UnkEmmyProtoState_l4fbd00 state;  // 0x29f26
    u8 pad_0x2a70c[4];
    UnkEmmyProtoGrab_l4fbd00 grab;  // 0x2a710

    bool CanGrab() { return reinterpret_cast<bool (*)(UnkEmmyProtoAI_l4fbd00*)>(vtable[0x818 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkEmmyProtoAI_l4fbd00, grab) == 0x2a710, "layout");

static inline UnkQualifiedId& TutoLaunchedId() {
    UnkProtoNamedId_A3_b66& def = reinterpret_cast<UnkProtoNamedId_A3_b66&>(g_unk_71726dd158);
    if (def.qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&def.qid, &def, g_unk_7101d470e0);
    return def.qid;
}

// Plays the slow-motion slide at the actor's position.
static inline void PlaySlideSound(UnkEmmyProtoActor_l4fbd00* actor) {
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = CFilePathStrId(UnkStrIdInline(), "actors/samus/slomoslide.wav", true);
    s_desc.field_0x6d = true;
    s_desc.position[0] = actor->transform[0][3];
    s_desc.position[1] = actor->transform[1][3];
    s_desc.position[2] = actor->transform[2][3];
    s_desc.field_0x38 = 1.5f;
    s_desc.field_0x3c = 3001.0f;
    s_desc.field_0x40 = 2996.0f;
    s_desc.field_0x44 = 1.0f;
    fn_7100d7c2f0(reinterpret_cast<UnkEntityWithSound*>(actor), reinterpret_cast<const u32*>(&s_desc), false, true,
                  true);
}

// Whether `actor` is further along x than `of`.
static inline bool IsAhead(const UnkEmmyProtoActor_l4fbd00* actor, const UnkEmmyProtoActor_l4fbd00* of) {
    f32 x = of->X();
    return actor->X() > x;
}

void fn_71004fbd00(UnkEmmyProtoAI_l4fbd00* self, f32 dt) {
    UnkEmmyProtoState_l4fbd00* state = &self->state;
    fn_710054d16c(self, dt);
    UnkEmmyProtoActor_l4fbd00* target = TargetOf(self->owner);
    bool ahead = target && IsAhead(target, self->owner);
    UnkEmmyProtoGrab_l4fbd00* grab = &self->grab;
    if (state->sliding) {
        bool running = fn_710002fb70(self->slide_timer) || state->active;
        if (ahead || !running)
            state->sliding = false;
    }

    if (ahead && self->CanGrab() && state->tutorial) {
        UnkGameManager* gm = g_unk_7101d47310;
        const CStrId& section = *static_cast<const CStrId*>(fn_7100df6d84(gm));
        gm = g_unk_7101d47310;
        UnkQualifiedId& id = TutoLaunchedId();
        bool launched = true;
        if (CBlackboard::CSection* s = fn_71010472e0(&gm->field_0x968, section)) {
            if (!gm->field_0x968.field_0xe8) {
                UnkTypedValue v;
                v.type = fn_710000d228();
                v.value = &launched;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&id), &v);
            }
        }
        if (grab->handle) {
            fn_71004fd510(grab->handle, grab);
            grab->handle = nullptr;
        }
    }

    if (state->tutorial) {
        UnkEmmyProtoParts_l4fbd00* parts = TargetOf(self->owner)->parts;
        UnkEmmyProtoPart_l4fbd00* part = nullptr;
        for (u32 i = 0; i < parts->count; ++i) {
            if (parts->parts[i]->kind == 3) {
                part = parts->parts[i];
                break;
            }
        }
        if (part) {
            if (!state->slomo && __builtin_fabsf(self->owner->X() - target->X()) < 220.0f) {
                fn_71004fb4ec(self, true);
                {
                    CRntString name("s");
                    fn_7100df6d3c(g_unk_7101d47310, &name, 0.2f, 0.3f);
                }
                auto* manager = reinterpret_cast<UnkSlomoManager_l4fbd00*>(g_unk_7101d47310);
                manager->callback_owner = self;
                manager->callback = fn_71004fd8cc;
                PlaySlideSound(self->owner);
                state->slomo = true;
            }
            bool hit = false;
            if (UnkEmmyProtoHit_l4fbd00* shape = target->parts->Hit()) {
                UnkEmmyProtoQuery_l4fbd00 query;
                void* collision = self->owner->collision;
                void* body = shape->shape;
                hit = fn_7100864e34(collision, 0x100000, body, shape->Transform(), &query, false);
            }
            if (fn_710002fb70(part->timer)) {
                if (hit)
                    fn_710002ffb4(reinterpret_cast<UnkGuiKeyList*>(part->timer), &TutoLaunchedId().id);
            } else {
                UnkVec3f pos = self->owner->Position();
                UnkVec3f target_pos = TargetOf(self->owner)->Position();
                f32 x = pos.x;
                f32 target_x = target_pos.x;
                const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(target->transform));
                bool passed = (m.m[2] < 0.0f && x > target_x) || (m.m[2] > 0.0f && x < target_x);
                if (passed && __builtin_fabsf(x - target_x) > g_unk_71726dcc88->pass_distance) {
                    fn_71004fb4ec(self, false);
                    state->tutorial = false;
                    fn_7101178e08(TargetOf(self->owner)->animation, 0x10, false, false, -1.0f);
                }
            }
        }
    }

    if (grab->cooldown > 0.0f) {
        grab->cooldown -= dt;
        if (grab->cooldown < 0.0f) {
            grab->cooldown = 0.0f;
            return;
        }
        UnkEmmyProtoActor_l4fbd00* owner = self->owner;
        if (owner->pushable && owner->move) {
            UnkEmmyProtoPush_l4fbd00 push;
            push.field_0x1c = 0;
            push.field_0x20 = 0;
            push.field_0x28 = 0;
            push.field_0x30 = 0;
            push.vtable = g_unk_7101a25b10 + 2;
            push.kind = 5;
            push.field_0x39 = 0;
            push.field_0x3d = 0;
            push.field_0x3a = 1;
            push.field_0x3b = 1;
            push.dir[0] = owner->move->dir[0];
            push.dir[1] = owner->move->dir[1];
            push.dir[2] = owner->move->dir[2];
            push.field_0x14 = 1;
            fn_71006f4b0c(owner->pushable, &push, 1);
        }
    }
}
