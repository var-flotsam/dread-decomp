// CChozoWarriorAIComponent unit (tools/tu_map.py): the component's update. With an animation, either waits
// for the shinespark-impact tweak track to end, or counts the guard timer down, runs the grab (sending its
// start, timing it out and passing the target's grapple-track progress on as "Factor"), faces the owner,
// and turns to a target behind it; then counts the second timer down (setting a timeline id when it runs
// out) and runs the base update. Names and layouts are placeholders.
#include <math.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAttackComponent.h"
#include "game/CChozoWarriorAIComponent.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU1730.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkTunable;
extern UnkTunable g_unk_71726d0e00;
extern u64 g_unk_71016499e8;  // the timeline id

struct UnkBossAI_l3ae15c;
struct UnkWarriorTurn_b58;
struct UnkCancelOwner_ovb0128;
void fn_71003ae15c(UnkBossAI_l3ae15c* self, f32 dt);
void fn_71004994f4(UnkWarriorTurn_b58* self);

// The warrior's tunables (g_unk_71726d0e00).
struct UnkChozoWarriorTunable_l4990ac {
    u8 pad_0x0[0x24];
    f32 grab_time;  // 0x24
};

static inline const UnkChozoWarriorTunable_l4990ac& Tunable() {
    return reinterpret_cast<const UnkChozoWarriorTunable_l4990ac&>(g_unk_71726d0e00);
}

// The records the two track queries find.
struct UnkTweakTrackRecord_l4990ac {
    u8 pad_0x0[0x20];
    bool playing;  // 0x20
};

struct UnkGrappleTrackRecord_l4990ac {
    u8 pad_0x0[0x10];
    f32 start;  // 0x10
    f32 end;    // 0x14
    u32 kind;   // 0x18
};

struct UnkChozoWarriorOwner_l4990ac {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38 (its translation column at +0x44, +0x54, +0x64)
    u8 pad_0x78[0x1a8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x50];
    void* attack;  // 0x278
    u8 pad_0x280[0x68];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkChozoWarriorOwner_l4990ac, animation) == 0x220, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorOwner_l4990ac, attack) == 0x278, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorOwner_l4990ac, timeline) == 0x2e8, "layout");

struct UnkChozoWarriorAttack_l4990ac {
    u8 pad_0x0[0x50];
};

struct UnkChozoWarriorGrab_l4990ac {
    bool active;   // 0x0
    bool started;  // 0x1
    u8 pad_0x2[2];
    f32 time;  // 0x4
};

struct UnkChozoWarriorAI_l4990ac {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner_l4990ac* owner;  // 0x30
    u8 pad_0x38[0x1110];
    UnkChozoWarriorAttack_l4990ac* state;  // 0x1148
    u8 pad_0x1150[0x13850];
    bool facing;  // 0x149a0
    u8 pad_0x149a1[0x77];
    UnkChozoWarriorGrab_l4990ac grab;  // 0x14a18
    u8 pad_0x14a20[0x10];
    UnkChozoWarriorAttack_l4990ac attack;  // 0x14a30
    u8 pad_0x14a80[0xb0];
    UnkChozoWarriorAttack_l4990ac* current;  // 0x14b30
    f32 guard_timer;                         // 0x14b38
    bool waiting;                            // 0x14b3c
    u8 pad_0x14b3d[0xb];
    f32 timer;  // 0x14b48
};
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l4990ac, state) == 0x1148, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l4990ac, facing) == 0x149a0, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l4990ac, grab) == 0x14a18, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l4990ac, attack) == 0x14a30, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l4990ac, current) == 0x14b30, "layout");
static_assert(__builtin_offsetof(UnkChozoWarriorAI_l4990ac, timer) == 0x14b48, "layout");

void fn_7100499838(UnkChozoWarriorAI_l4990ac* self);
void fn_7101105990(UnkChozoWarriorAI_l4990ac* self, UnkChozoWarriorAttack_l4990ac* attack, bool flag);
void fn_7101105e14(UnkChozoWarriorAI_l4990ac* self, f32 value);

static inline char* TeamManager() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return instance;
}

// True when the target is behind the owner (against the owner's forward axis).
struct UnkPos_l4990ac {
    f32 x, y, z;
};

static inline UnkPos_l4990ac operator-(const UnkPos_l4990ac& a, const UnkPos_l4990ac& b) {
    UnkPos_l4990ac d = {a.x - b.x, a.y - b.y, a.z - b.z};
    return d;
}

static inline UnkPos_l4990ac Pos(const UnkActor* actor) {
    UnkPos_l4990ac p = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    return p;
}

static inline UnkPos_l4990ac Pos(const UnkChozoWarriorOwner_l4990ac* owner) {
    UnkPos_l4990ac p = {owner->transform.m[3], owner->transform.m[7], owner->transform.m[11]};
    return p;
}

static inline bool TargetBehind(UnkChozoWarriorAI_l4990ac* self, UnkActor* target) {
    UnkPos_l4990ac d = Pos(target) - Pos(self->owner);
    f32 inv = 1.0f / (sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
    f32 dx = d.x * inv;
    f32 dy = d.y * inv;
    f32 dz = d.z * inv;
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    return fx * dx + dy * fy + dz * fz < 0.0f;
}

void fn_71004990ac(UnkChozoWarriorAI_l4990ac* self, f32 dt) {
    UnkTrackQuery query;
    if (UnkAnimationComponent* animation = self->owner->animation) {
        if (self->waiting) {
            query.cls = fn_7100c624f8();
            query.result = nullptr;
            query.value = -1.0f;
            fn_710117b08c(animation, &query);
            if (!query.result || !reinterpret_cast<UnkTweakTrackRecord_l4990ac*>(query.result)->playing) {
                self->waiting = false;
                fn_71004994f4(reinterpret_cast<UnkWarriorTurn_b58*>(self));
            }
        } else {
            self->facing = true;
            if (self->guard_timer > 0.0f) {
                self->guard_timer -= dt;
                if (self->guard_timer <= 0.0f)
                    self->guard_timer = 0.0f;
            }
            UnkChozoWarriorGrab_l4990ac& grab = self->grab;
            if (grab.active) {
                if (grab.started) {
                    grab.started = false;
                    base::reflection::refl_EmitU32(animation, 0xc70c86aaade0d244, 0xf4061ff25a2163d6, 2,
                                                   &g_unk_710164b958, &g_unk_7172769740);
                }
                grab.time += dt;
                if (grab.time >= Tunable().grab_time) {
                    CChozoWarriorAIComponent_ReleaseHandleAndClearTimer(reinterpret_cast<UnkChozoWarriorAI733*>(self));
                } else if (UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))) {
                    if (UnkAnimationComponent* target_animation =
                            static_cast<UnkAnimationComponent*>(target->field_0x220)) {
                        // The target's grapple-track progress goes to the animation as "Factor".
                        query.cls = fn_7100d001f8();
                        query.result = nullptr;
                        query.value = -1.0f;
                        fn_710117b08c(target_animation, &query);
                        if (query.result) {
                            UnkGrappleTrackRecord_l4990ac* track =
                                reinterpret_cast<UnkGrappleTrackRecord_l4990ac*>(query.result);
                            f32 factor = track->kind == 1 ? query.value / (track->end - track->start) : 0.0f;
                            unk_prop_bag::fn_710033187c(
                                reinterpret_cast<unk_prop_bag::UnkPropBag*>(animation->field_0x80),
                                CStrId("Factor", true), factor);
                        }
                    }
                }
            }
            fn_7100498db8(reinterpret_cast<UnkSelf*>(self));
            fn_7100499838(self);
            void* attack = self->owner->attack;
            if (attack && !fn_7101136308(attack)) {
                UnkChozoWarriorAttack_l4990ac* state = self->state;
                if (state == &self->attack) {
                    UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
                    if (target && TargetBehind(self, target)) {
                        fn_71011a75f0(TeamManager(), reinterpret_cast<UnkCancelOwner_ovb0128*>(self->owner));
                        fn_7101105990(self, state, false);
                    }
                }
            }
            if (self->current)
                fn_7101105e14(self, 0.0f);
        }
    }
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer <= 0.0f) {
            self->timer = 0.0f;
            if (UnkTimelineComponent* timeline = self->owner->timeline)
                fn_7100a2b7b8(timeline, &g_unk_71016499e8);
        }
    }
    fn_71003ae15c(reinterpret_cast<UnkBossAI_l3ae15c*>(self), dt);
}
