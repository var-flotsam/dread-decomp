// CHecathonAIComponent's unit (tools/tu_map.py unit 865): fn_71005abb48 handles a message after the base
// handler (fn_710036974c, whose result it returns). With the message flag (+0x8) set: unless the plankton
// cone track (CCharClassStopPlanktonConeTrack) plays, the state flag (+0x147cc) is cleared, fn_71005ab080
// runs and the plankton FX gets the tunable's +0x28 (as fn_71005a9fd8); the "airexpel" effect's flag (its
// +0x160, +0x78, +0x258) is remembered at +0x147d2 and the effect switched off. Without it: unless the track
// plays or the flag at +0x147c8 is set, the state flag is set and fn_71005ab414 runs; the effect is switched
// on again when remembered. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CFXComponent.h"
#include "game/CHecathonPlanktonFXComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

using base::reflection::UnkTunable;
// As src/game/CHecathonAIComponent/CHecathonAIComponent_Timeline_b60.cpp declares it.
extern UnkTunable g_unk_71726e8d00;

struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;

struct UnkRefl_fn_71005aad7c;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71005aad7c>(void*);
struct UnkPlanktonFX_A3_b71;

struct UnkWorld;
struct UnkPart;
struct UnkObject;

struct UnkHecathonMsg_ovb0131;
void fn_71005ab080(UnkHecathonMsg_ovb0131* self, bool flag);
void fn_71005ab414(UnkHecathonMsg_ovb0131* self, bool flag);

struct UnkHecathonFXData_ovb0131 {
    u8 pad_0x0[0x258];
    bool flag;  // 0x258
};

struct UnkHecathonFXSettings_ovb0131 {
    u8 pad_0x0[0x78];
    UnkHecathonFXData_ovb0131* data;  // 0x78
};

struct UnkHecathonEffect_ovb0131 {
    u8 pad_0x0[0xb];
    u8 id;  // 0xb
    u8 pad_0xc[0x154];
    UnkHecathonFXSettings_ovb0131* settings;  // 0x160
};

struct UnkHecathonOwner_ovb0131 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x58];
    UnkWorld* fx;  // 0x280
};

struct UnkHecathonMessage_ovb0131 {
    u8 pad_0x0[0x8];
    bool flag;  // 0x8
};

struct UnkHecathonState_ovb0131 {
    bool busy;  // 0x147c8
    u8 pad_0x1[0x3];
    bool active;  // 0x147cc
    u8 pad_0x5[0x5];
    bool effect_on;  // 0x147d2
};

struct UnkHecathonMsg_ovb0131 {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u8 pad_0x20[0x10];
    UnkHecathonOwner_ovb0131* owner;  // 0x30
    u8 pad_0x38[0x147c8 - 0x38];
    UnkHecathonState_ovb0131 state;  // 0x147c8
};

// The "airexpel" effect when its id is live in the FX list.
static inline UnkHecathonEffect_ovb0131* Live(UnkWorld* fx, UnkHecathonEffect_ovb0131* effect) {
    return fn_710114e668(reinterpret_cast<UnkObject*>(fx), effect->id) ? effect : nullptr;
}

u32 fn_71005abb48(UnkHecathonMsg_ovb0131* self, UnkHecathonMessage_ovb0131* msg) {
    UnkHecathonState_ovb0131* state = &self->state;
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    UnkAnimationComponent* animation = self->owner->animation;
    bool flag = msg->flag;
    bool playing = fn_710117b348(animation, fn_7100c50d58());
    if (flag) {
        if (!playing) {
            state->active = false;
            fn_71005ab080(self, false);
            fn_710080c5d4(reinterpret_cast<UnkPlanktonFX_A3_b71*>(
                              base::reflection::refl_FindTyped<UnkRefl_fn_71005aad7c>(self->field_0x18)),
                          *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726e8d00) + 0x28));
        }
        UnkWorld* fx = self->owner->fx;
        auto* effect = reinterpret_cast<UnkHecathonEffect_ovb0131*>(fn_7101152b90(fx, fn_7100001570("airexpel", 8)));
        effect = Live(fx, effect);
        state->effect_on = effect->settings->data->flag;
        fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), false);
    } else {
        if (!playing && !state->busy) {
            state->active = true;
            fn_71005ab414(self, false);
        }
        if (state->effect_on) {
            UnkWorld* fx = self->owner->fx;
            auto* effect =
                reinterpret_cast<UnkHecathonEffect_ovb0131*>(fn_7101152b90(fx, fn_7100001570("airexpel", 8)));
            if (effect)
                effect = Live(fx, effect);
            fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), true);
        }
    }
    return result;
}
