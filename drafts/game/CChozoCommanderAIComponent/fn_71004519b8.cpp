// CChozoCommanderAIComponent unit (tools/tu_map.py unit 665): the damage handler. While grabbing (+0x15d70)
// it only restarts the player's entry at +0x150d0 (unless an ultimate grab runs) and answers 1; otherwise,
// after the base handler, a shielded commander (+0x15cfc) soaks the hit (counting it, remembering where
// it landed in its own frame and setting a timeline id), else the hit is scaled by the char class's
// factor and capped so the commander keeps its ground, and a hit that drops it below the stage line
// makes it and its target play the stage-change request. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIAttackComponent.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/CGrabComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU54.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern const u64 g_unk_71016496f0;

void fn_710113b8cc(void* grab);

struct UnkCommanderPlayer_l4519b8 {
    void** vtable;
    u8 pad_0x8[0x198];
    UnkReflObject* current;  // 0x1a0

    void vf_0x178() { reinterpret_cast<void (*)(UnkCommanderPlayer_l4519b8*)>(vtable[0x178 / 8])(this); }
};

struct UnkCommanderGround_l4519b8 {
    u8 pad_0x0[0x50];
    f32 bottom;  // 0x50
    f32 top;     // 0x54
};

struct UnkStageLimits_l4519b8 {
    u8 pad_0x0[0x94];
    f32 field_0x94;
    f32 field_0x98;
};

struct UnkCommanderOwner_l4519b8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x140];
    UnkCommanderGround_l4519b8* ground;  // 0x1b8
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x38];
    void* grab;  // 0x260
    u8 pad_0x268[0x10];
    UnkCommanderPlayer_l4519b8* player;  // 0x278
    u8 pad_0x280[0x68];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkCommanderOwner_l4519b8, ground) == 0x1b8, "");
static_assert(__builtin_offsetof(UnkCommanderOwner_l4519b8, timeline) == 0x2e8, "");

struct UnkHitSource_l4519b8 {
    void** vtable;

    const UnkMatrix44* vf_0x70() {
        return reinterpret_cast<const UnkMatrix44* (*)(UnkHitSource_l4519b8*)>(vtable[0x70 / 8])(this);
    }
};

struct UnkCommanderHit_l4519b8 {
    u8 pad_0x0[0x20];
    bool handled;  // 0x20
    u8 pad_0x21[0x3f];
    UnkHitSource_l4519b8* source;  // 0x60
    u8 pad_0x68[0x10];
    f32 field_0x78;
    f32 field_0x7c;
    f32 field_0x80;
    u8 pad_0x84[0x30];
    u32 flags;  // 0xb4
    u8 pad_0xb8[0x40];
    f32 damage;  // 0xf8
};
static_assert(__builtin_offsetof(UnkCommanderHit_l4519b8, damage) == 0xf8, "");

struct UnkCommanderCharClass_l4519b8 {
    u8 pad_0x0[0x15c0];
    u8 factors[0x70];      // 0x15c0
    u8 factors_alt[0x70];  // 0x1630
};

struct UnkCommanderAI_l4519b8 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_l4519b8* owner;  // 0x30
    u8 pad_0x38[0x1108];
    u32 field_0x1140;
    u8 pad_0x1144[0x1385c];
    UnkCommanderCharClass_l4519b8* char_class;  // 0x149a0
    u8 pad_0x149a8[0x728];
    u8 entry_0x150d0[0x720];
    u8 entry_0x157f0[0x508];
    f32 soaked;        // 0x15cf8
    bool shielded;     // 0x15cfc
    u8 pad_0x15cfd[0x3];
    f32 field_0x15d00;
    u8 pad_0x15d04;
    bool can_soak;     // 0x15d05
    u8 pad_0x15d06[0xe];
    f32 field_0x15d14;
    u8 pad_0x15d18[0x58];
    bool grabbing;     // 0x15d70
    u8 pad_0x15d71[0x147];
    bool may_switch;   // 0x15eb8
    bool switched;     // 0x15eb9
    u8 pad_0x15eba[0x2e];
    UnkVec3f hit_pos;  // 0x15ee8
    f32 field_0x15ef4;
    f32 field_0x15ef8;
    f32 field_0x15efc;
};
static_assert(__builtin_offsetof(UnkCommanderAI_l4519b8, char_class) == 0x149a0, "");
static_assert(__builtin_offsetof(UnkCommanderAI_l4519b8, entry_0x157f0) == 0x157f0, "");
static_assert(__builtin_offsetof(UnkCommanderAI_l4519b8, grabbing) == 0x15d70, "");
static_assert(__builtin_offsetof(UnkCommanderAI_l4519b8, hit_pos) == 0x15ee8, "");

static inline UnkVec3f Translation(const UnkMatrix44& m) {
    return UnkVec3f(m.m[3], m.m[7], m.m[11]);
}

// Sends `target` the request `hash` with mode 0 (as game_s2/UnkSpitSend.h, with the caller's arguments).
static inline void Send(void* target, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

static const u64 kShieldHitId = 0x8d1b8dfce0a09065;

u32 fn_71004519b8(UnkCommanderAI_l4519b8* self, UnkCommanderHit_l4519b8* hit) {
    if (self->grabbing) {
        UnkCommanderPlayer_l4519b8* player = self->owner->player;
        // The player's entry is read before the player is checked, as in the original.
        UnkReflObject* current = player->current;
        bool ultimate = current && fn_710009d50c(current->vf_0x0(), fn_710043d5e8());
        if (player && !ultimate) {
            void* grab = self->owner->grab;
            if (grab && fn_710113d83c(grab))
                fn_710113b8cc(grab);
            self->owner->player->vf_0x178();
            fn_7101134420(reinterpret_cast<UnkCommanderPlayer_b67a2*>(self->owner->player), self->entry_0x150d0,
                          0, 0, nullptr, -1.0f);
        }
        return 1;
    }

    if (u32 result = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self),
                                   reinterpret_cast<UnkQuetzoaXMessage_b58*>(hit)))
        return result;

    if (self->field_0x1140 == 0) {
        UnkCommanderOwner_l4519b8* owner = self->owner;
        if (UnkCommanderGround_l4519b8* ground = owner->ground) {
            if (self->shielded) {
                if (!self->can_soak)
                    return 2;
                self->soaked += hit->damage;
                self->field_0x15d00 = 1.0f;
                u64 id = kShieldHitId;
                UnkTimelineComponent* timeline = owner->timeline;
                if (timeline && !fn_7100a2be94(timeline, &id)) {
                    UnkVec3f* pos = &self->hit_pos;
                    *pos = Translation(self->owner->transform);
                    if (hit->source) {
                        UnkMatrix44 inverse = fn_7100078314(&fn_710002614c(&self->owner->transform));
                        UnkVec3f at = Translation(*hit->source->vf_0x70());
                        *pos = TransformPointByMatrixWithPerspectiveDivide(reinterpret_cast<const UnkMat4f*>(&inverse), &at);
                    }
                    self->field_0x15ef4 = hit->field_0x78;
                    self->field_0x15ef8 = hit->field_0x7c;
                    self->field_0x15efc = hit->field_0x80;
                    UnkTimelineComponent* t = self->owner->timeline;
                    u32 mask = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(t, &id, &mask, false, -1.0f);
                }
                return 1;
            }

            f32 bottom = ground->bottom;
            f32 top = ground->top;
            const UnkStageLimits_l4519b8* limits = reinterpret_cast<const UnkStageLimits_l4519b8*>(&g_unk_717272fd38);
            f32 low = limits->field_0x94;
            f32 high = limits->field_0x98;
            bool alt = self->switched;
            if (!alt && fn_7101105970(reinterpret_cast<UnkScorpiusAI_b69a4*>(self)) == self->entry_0x157f0 &&
                self->may_switch) {
                self->switched = true;
                alt = true;
            }
            f32 damage = hit->damage;
            UnkCommanderCharClass_l4519b8* char_class = self->char_class;
            u8* factors = alt ? char_class->factors_alt : char_class->factors;
            f32 factor = fn_71008c2de8(reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(factors), hit->flags);
            fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), damage * factor);
            f32 limit = top - (bottom + (low + high) * -0.5f);
            fn_71008c1538(reinterpret_cast<UnkRequest*>(hit), limit < hit->damage ? limit : hit->damage);
            hit->handled = true;
            UnkAnimationComponent* animation = self->owner->animation;
            if (animation && fn_710117b348(animation, fn_7100bf8dc8()) && top - hit->damage < bottom - low) {
                u64 request = fn_710117ae38(animation, 0x46b21f327512700c, false) ? 0x344e6b8e8781639b
                                                                                   : 0xb1908e0ac25fa340;
                base::reflection::UnkReflTypeIdArg mode;
                base::reflection::UnkReflTypeIdArg mask;
                Send(animation, request, &mode, &mask);
                UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
                if (target && fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x220) {
                    base::reflection::UnkReflTypeIdArg target_mode;
                    base::reflection::UnkReflTypeIdArg target_mask;
                    Send(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x220, request,
                         &target_mode, &target_mask);
                }
            }
        }
    }

    if (hit->flags & 0x20) {
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            u32 mask = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_71016496f0, &mask, false, -1.0f);
            self->field_0x15d14 = 2.5f;
        }
    }
    return 0;
}
