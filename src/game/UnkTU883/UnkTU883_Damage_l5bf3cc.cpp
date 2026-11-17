// CHydrogigaAIComponent's unit (tools/tu_map.py unit 883): the damage handler. Unless the base handler answers,
// a hit on one of the named colliders is scaled by the class data's damage factor for it and updates the
// component's timers; two of them then play a timeline event on the owner. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CBossAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/CCoreXAIComponent.h"
#include "game/CDamageSourceFactor.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU883.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726eb428 __attribute__((visibility("hidden")));

// The collider a hit landed on: its group name at +0x20.
struct UnkHydrogigaHitCollider_l5bf3cc {
    u8 pad_0x0[0x20];
    CStrId group;
};

struct UnkHydrogigaHit_l5bf3cc {
    u8 pad_0x0[0x60];
    UnkHydrogigaHitCollider_l5bf3cc* collider;  // +0x60
    u8 pad_0x68[0x4c];
    u32 flags;  // +0xb4
    u8 pad_0xb8[0x40];
    f32 damage;  // +0xf8
};

// The damage factor tables of CCharClassHydrogigaAIComponent.
struct UnkHydrogigaCharClass_l5bf3cc {
    u8 pad_0x0[0x578];
    u8 field_0x578[0x658 - 0x578];
    u8 field_0x658[0x6c8 - 0x658];
    u8 field_0x6c8[0x8];
};

struct UnkHydrogigaOwner_l5bf3cc {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x78];
    UnkCollisionContainer* colliders;  // +0x2a0
    u8 pad_0x2a8[0x40];
    UnkTimelineComponent* timeline;  // +0x2e8
};

struct UnkHydrogigaAI_l5bf3cc {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner_l5bf3cc* owner;  // +0x30
    u8 pad_0x38[0x149a0 - 0x38];
    UnkHydrogigaCharClass_l5bf3cc* char_class;  // +0x149a0
    u8 pad_0x149a8[0x14];
    f32 field_0x149bc;
    bool field_0x149c0;
    u8 pad_0x149c1[0xf];
    f32 field_0x149d0;
    u8 pad_0x149d4[4];
    f32 field_0x149d8;
};
static_assert(__builtin_offsetof(UnkHydrogigaAI_l5bf3cc, field_0x149d8) == 0x149d8, "layout");

bool fn_71005bf75c(UnkHydrogigaAI_l5bf3cc* self, UnkHydrogigaHit_l5bf3cc* hit);

// Scales the hit's damage by the factor `table` gives for its flags.
static inline void ScaleDamage(UnkHydrogigaHit_l5bf3cc* hit, u8* table) {
    f32 damage = hit->damage;
    fn_71008c1538(reinterpret_cast<UnkRequest*>(hit),
                  damage * fn_71008c2de8(reinterpret_cast<UnkRobotSoldierTable_A2_b69*>(table), hit->flags));
}

// Plays the timeline event id on every channel.
static inline void PlayEvent(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

u32 fn_71005bf3cc(UnkHydrogigaAI_l5bf3cc* self, UnkHydrogigaHit_l5bf3cc* hit) {
    u32 result = fn_71003adaf8(reinterpret_cast<UnkQuetzoaXAI_b58*>(self), reinterpret_cast<UnkQuetzoaXMessage_b58*>(hit));
    if (result)
        return result;
    u64 event = -1;
    UnkHydrogigaOwner_l5bf3cc* owner = self->owner;
    void* animation = owner->field_0x220;
    UnkCollisionContainer* colliders = owner->colliders;
    char* target = fn_71004dad10(reinterpret_cast<UnkActor*>(owner));
    if (!animation || !colliders || !target)
        return 2;
    {
        CStrId group = hit->collider ? hit->collider->group : g_unk_7101d470e0;
        CStrId name = hit->collider ? *fn_7100864018(colliders, hit->collider) : g_unk_7101d470e0;
        if (group == g_unk_71726eb450) {
            ScaleDamage(hit, self->char_class->field_0x578);
            if (self->field_0x149c0) {
                result = 2;
            } else {
                if (self->field_0x149bc > 0.0f) {
                    self->field_0x149bc -= hit->damage;
                    event = 0xfffaba557b95b8d5;
                }
                result = 1;
            }
        } else if (group == g_unk_71726eb440) {
            if (hit->flags & 0x2800041c) {
                ScaleDamage(hit, self->char_class->field_0x658);
                self->field_0x149d0 = hit->damage + self->field_0x149d0;
                if (self->field_0x149d8 <= 0.5f)
                    self->field_0x149d8 = 1.0f;
                event = 0x72c8d52fee0ec0d6;
                result = 0;
            } else {
                result = 2;
            }
        } else if (name == g_unk_71726eb428) {
            ScaleDamage(hit, self->char_class->field_0x6c8);
            result = fn_71005bf75c(self, hit) ? 1 : 2;
        } else {
            result = 2;
        }
    }
    UnkTimelineComponent* timeline = self->owner->timeline;
    if (timeline && event != (u64)-1)
        PlayEvent(timeline, &event);
    return result;
}
