// behaviortree::CScorpiusRegereratingMaskAction's unit: updates the node. It scales the tunable value
// (g_unk_7172790a08 +0xac) with how far the mask has regenerated and, while the owner is idle, walks the
// action through its stages (+0x2c): 0 waits for the AI to allow it and turns the owner's target away,
// 1 waits for the regeneration time, 2 waits and then either sends the animation a request or tells the AI
// to go on. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAttackComponent.h"
#include "game/CCharClassScorpiusAIComponent.h"
#include "game/CCharClassTweakPlayRateTrack.h"
#include "game/CEnemyMovement.h"
#include "game/CScorpiusAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU27.h"
#include "game/UnkTU53.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkScorpiusKey_b59a4;
extern UnkScorpiusKey_b59a4 g_unk_7172703bc0 __attribute__((visibility("hidden")));
extern UnkScorpiusKey_b59a4 g_unk_7172703bc8 __attribute__((visibility("hidden")));
extern u64 g_unk_710164a538;
extern u64 g_unk_710164a540;
void fn_710069408c(UnkScorpiusAI_b59a4* self);

struct UnkScorpiusCharClass_l68d3b8 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x590 - 0x8];
    f32 field_0x590;
    u8 pad_0x594[4];
    base::global::CStrId field_0x598;
    f32 field_0x5a0;
    f32 field_0x5a4;
};

struct UnkScorpiusAI_l68d3b8 : base::reflection::UnkReflObject {
    u8 pad_0x8[8];
    UnkScorpiusCharClass_l68d3b8* charClass;  // 0x10
};

struct UnkMaskTunable_l68d3b8 {
    u8 pad_0x0[0xac];
    f32 field_0xac;
};

struct UnkScorpiusTunable_l68d3b8 {
    u8 pad_0x0[0x38];
    f32 field_0x38;
};

struct UnkMaskOwner_l68d3b8 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x218 - 0x78];
    void* movement;                 // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkScorpiusAI_l68d3b8* ai;  // 0x240
    u8 pad_0x248[0x10];
    void* field_0x258;
    u8 pad_0x260[0x38];
    void* navigation;  // 0x298
    u8 pad_0x2a0[0x48];
    UnkTimelineComponent* timeline;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkMaskOwner_l68d3b8, timeline) == 0x2e8, "layout");

struct UnkMaskUpdate_l68d3b8 {
    u8 pad_0x0[0x18];
    UnkMaskOwner_l68d3b8* owner;  // 0x18
    u8 pad_0x20[0xc];
    s32 stage;  // 0x2c
    f32 timer0;  // 0x30
    f32 timer1;  // 0x34
};

// A kind-5 block whose direction (+0x8, flag +0x14) is set, with the vtable g_unk_7101a24b58.
struct UnkMaskDirParams_l68d3b8 {
    void** vtable;
    f32 x;
    f32 y;
    f32 z;
    bool has_direction;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
};

template <class T>
static inline T* Cast_l68d3b8(T* object, const base::reflection::CClass* (*get_class)()) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), get_class()) ? object : nullptr;
}

u32 fn_710068d3b8(UnkMaskUpdate_l68d3b8* self, f32 dt) {
    UnkMaskDirParams_l68d3b8 params;
    u32 timelineMask;
    f32 value;
    base::reflection::UnkReflTypeIdArg mode, mask;
    UnkMaskTunable_l68d3b8* tunable = reinterpret_cast<UnkMaskTunable_l68d3b8*>(&g_unk_7172790a08);
    f32 speed = 1.0f;
    UnkScorpiusAI_l68d3b8* ai = self->owner->ai;
    UnkScorpiusCharClass_l68d3b8* charClass = nullptr;
    bool ready = false;
    if (ai) {
        if (!fn_710009d50c(ai->vf_0x0(), fn_7100690c74())) {
            ai = nullptr;
        } else if ((charClass = ai->charClass)) {
            if (!fn_710009d50c(charClass->vf_0x0(), fn_710068f63c())) {
                charClass = nullptr;
            } else {
                tunable->field_0xac = 1.0f;
                f32 total = charClass->field_0x5a0 + charClass->field_0x5a4;
                f32 done = 0.0f;
                if (self->stage == 2) {
                    f32 v = charClass->field_0x5a0 + self->timer0;
                    if (v > 0.0f)
                        done = v >= total ? total : v;
                } else if (self->stage == 1) {
                    f32 v = self->timer1;
                    if (v > 0.0f)
                        done = v >= total ? total : v;
                }
                f32 ratio = done / total;
                if (charClass->field_0x598.data != &g_unk_7101d47108->empty)
                    ratio = fn_7100076020(ratio, charClass->field_0x598.Text());
                f32 range = (charClass->field_0x590 > 1.0f ? charClass->field_0x590 : 1.0f) - 1.0f;
                speed = ratio * range + 1.0f;
                ready = true;
            }
        }
    }
    tunable->field_0xac = speed;
    void* movement = self->owner->movement;
    if (!movement || !ready || fn_71010cab0c(movement))
        return 2;
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return 2;
    switch (self->stage) {
    case 0: {
        value = 0.0f;
        if (!fn_71010fc68c(reinterpret_cast<UnkGoliathAI_b58a3*>(self->owner->ai), &value))
            return 2;
        if (value > reinterpret_cast<UnkScorpiusTunable_l68d3b8*>(&g_unk_7172703e88)->field_0x38)
            return 2;
        if (!fn_710069409c(reinterpret_cast<UnkScorpiusAI_b59a4*>(ai),
                           reinterpret_cast<const UnkScorpiusKey_b59a4*>(&g_unk_7172703bc0), false, -1.0f, -1.0f))
            return 2;
        self->stage = 1;
        self->timer1 = 0.0f;
        UnkTimelineComponent* timeline = self->owner->timeline;
        if (timeline) {
            fn_7100a2b7b8(timeline, &g_unk_710164a538);
            timelineMask = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_710164a540, &timelineMask, false, -1.0f);
        }
        UnkMaskOwner_l68d3b8* owner = self->owner;
        if (owner) {
            if (void* navigation = owner->navigation) {
                params.field_0x1c = 0;
                params.field_0x34 = 0;
                __builtin_memset(params.field_0x20, 0, 16);
                params.field_0x30 = 0;
                params.vtable = g_unk_7101a24b58 + 2;
                const UnkMatrix44& m = fn_710002614c(&owner->xform);
                params.x = -m.m[2];
                params.y = -m.m[6];
                params.z = -m.m[10];
                params.has_direction = true;
                fn_71006f4b0c(navigation, &params, 0);
            }
        }
        break;
    }
    case 1:
        if (self->timer1 < 0.0f)
            break;
        self->timer1 += dt;
        if (self->timer1 <= charClass->field_0x5a0)
            break;
        if (!fn_710117b348(animation, fn_7100c0d1d8()) && !fn_7101136308(self->owner->field_0x258)) {
            if (!fn_710117b348(animation, fn_7100c4b288()))
                break;
            self->timer1 = -1.0f;
        }
        self->stage = 2;
        break;
    case 2: {
        self->timer0 += dt;
        f32 limit = charClass->field_0x5a4 + 0.5f;
        fn_7101136308(self->owner->field_0x258);
        if (self->timer0 < limit)
            break;
        if (fn_7101136308(self->owner->field_0x258))
            break;
        if (fn_710117b348(self->owner->animation, fn_7100c0d1d8()))
            break;
        UnkActor* target = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner));
        UnkActor* body = reinterpret_cast<UnkActor*>(self->owner);
        f32 dx = target->field_0x44 - body->field_0x44;
        f32 dy = target->field_0x54 - body->field_0x54;
        f32 dz = target->field_0x64 - body->field_0x64;
        const UnkMatrix44& m = fn_710002614c(&self->owner->xform);
        if (dx * m.m[2] + dy * m.m[6] + dz * m.m[10] < 0.0f) {
            mode.id = 1;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0xf9ddf18a72f51424ull, 0xf4061ff25a2163d6ull, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
            self->stage = 1;
            self->timer0 = 0.0f;
            self->timer1 = 0.0f;
        } else if (fn_710069409c(reinterpret_cast<UnkScorpiusAI_b59a4*>(ai),
                                 reinterpret_cast<const UnkScorpiusKey_b59a4*>(&g_unk_7172703bc8), true, -1.0f,
                                 -1.0f)) {
            self->stage = 3;
            fn_710069408c(reinterpret_cast<UnkScorpiusAI_b59a4*>(ai));
        }
        break;
    }
    }
    return 2;
}
