// Callers that request a state from a component's small state machine (the template of
// src/game/UnkStateMachineSet_b60.cpp, repeated here under its own names so both units link): the request
// stores the pending id, gives up past the last state or while busy, and otherwise switches state.
// Batch 61 callers: CEmmyGrabSamusAttack (machine at +0x28, states 0..13, "none" 14) and
// CHydrogigaAIComponent. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEmmyGrabSamusAttack.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU811.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game_s2/UnkChargeComponent.h"
#include "types.h"

struct UnkSmState_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b61* __attribute__((aligned(4))) UnkSmStatePtr_b61;

struct UnkSmMachine_b61 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b61* previous, UnkSmState_b61* next);
    virtual void vf_0x28(UnkSmState_b61* current, UnkSmState_b61* next);

    u64 field_0x8;
    UnkSmState_b61* current;
    UnkSmState_b61* previous;
    u32 count;
    UnkSmStatePtr_b61 states[16];
    u32 pending;
    bool busy;
};

template <bool Launder>
__attribute__((always_inline)) static inline bool FindState_b61(UnkSmMachine_b61* self, UnkSmStatePtr_b61* states,
                                                               u32 id, UnkSmState_b61*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b61);
    asm("" : "+r"(n));
    UnkSmStatePtr_b61* p = states ? states : self->states;
    if (Launder)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b61)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 None, bool Launder = false>
__attribute__((always_inline)) static inline void SwitchState_b61(UnkSmMachine_b61* self, UnkSmStatePtr_b61* states,
                                                                 u32 id, UnkSmState_b61* current) {
    if (!current || current->id != id) {
        UnkSmState_b61* found;
        UnkSmState_b61* next;
        u32 enter;
        if (FindState_b61<Launder>(self, states, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            asm("" : "+r"(enter));
            next = nullptr;
            asm("" : "+r"(next));
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            typedef void (*EnteredFn)(UnkSmState_b61*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None, bool Launder = false>
__attribute__((always_inline)) static inline void RequestStateAt_b61(UnkSmMachine_b61* self,
                                                                    UnkSmStatePtr_b61* states, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b61<None, Launder>(self, states, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_b61(UnkSmMachine_b61* self, u32 id) {
    RequestStateAt_b61<Last, None>(self, nullptr, id);
}

// ---- CEmmyGrabSamusAttack (tools/tu_map.py) ----
struct UnkOwner;
struct UnkPlayer;

struct UnkGrabPart_b61 {
    u8 pad_0x0[0x5c];
    bool field_0x5c;
};
struct UnkGrabPlayer_b61 {
    u8 pad_0x0[0x1c0];
    UnkGrabPart_b61* field_0x1c0;
};

struct UnkGrabSm_b61 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x28 - 0x10];
    UnkSmMachine_b61 machine;
    u8 pad_0xd8[0x6b4 - 0xd8];
    f32 timer;
    u8 pad_0x6b8[4];
    f32 field_0x6bc;
};

void fn_710050c490(UnkGrabSm_b61* self);

// Grab hold: while the target has no "ignore melee" track, counts down and then asks for state 10.
void fn_710050d67c(UnkGrabSm_b61* self, f32 dt) {
    self->timer += dt;
    UnkActor* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310));
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(actor->field_0x220), fn_7100d0ebf8()))
        return;
    fn_710050c490(self);
    self->field_0x6bc -= dt;
    if (!(self->field_0x6bc <= 0.0f))
        return;
    RequestState_b61<13, 14>(&self->machine, 10);
}

// State 13 or 12 by the player's part flag.
void fn_710050add4(UnkGrabSm_b61* self) {
    auto* player = reinterpret_cast<UnkGrabPlayer_b61*>(fn_7100dec788(g_unk_7101d47310));
    if (player->field_0x1c0->field_0x5c)
        RequestState_b61<13, 14>(&self->machine, 13);
    else
        RequestState_b61<13, 14>(&self->machine, 12);
}

// Release: once the timer passes 0.18 s, or the owner's E.M.M.I. AI (+0x240, a CEmmyAIComponent) has its
// flag at +0x2a6c0 set, clears that flag, fades in (fn_7100df6c78), and unless the lazily created manager
// (g_unk_7101d47360) is in state 2, re-enables the player's input (+0x1d8) and asks for state 4.
struct UnkGrabAI_b61 {
    void** vtable;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGrabAI_b61*)>(vtable[0])(this);
    }
};
struct UnkGrabManager_b61 {
    u8 pad_0x0[0x110];
    s32 state;
};
struct UnkManager;
extern UnkManager* g_unk_7101d47360;
void fn_7100ee1f34(void*);
struct UnkInputComponent;
struct UnkGrabInputArg_b61;
extern UnkGrabInputArg_b61 g_unk_71726de5d0 __attribute__((visibility("hidden")));

void fn_710050be58(UnkGrabSm_b61* self, f32 dt) {
    self->timer += dt;
    auto* ai = static_cast<UnkGrabAI_b61*>(self->owner->field_0x240);
    char* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<char*>(ai) : nullptr;
    bool* flag;
    if (self->timer > 0.18f) {
        flag = reinterpret_cast<bool*>(emmy + 0x2a6c0);
    } else {
        if (!emmy)
            return;
        flag = reinterpret_cast<bool*>(emmy + 0x2a6c0);
        if (!*flag)
            return;
    }
    *flag = false;
    fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.0f);
    void* manager = reinterpret_cast<void*&>(g_unk_7101d47360);
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        reinterpret_cast<void*&>(g_unk_7101d47360) = manager;
    }
    if (static_cast<UnkGrabManager_b61*>(manager)->state == 2)
        return;
    if (auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310)))
        if (player->field_0x1d8)
            fn_7100872460(static_cast<UnkInputComponent*>(player->field_0x1d8), false, false, &g_unk_71726de5d0);
    RequestState_b61<13, 14>(&self->machine, 4);
}

struct UnkEmmyAI_b61;

// Grab hold, second form: after the countdown, state 9 when the E.M.M.I. AI's value (fn_7100552ee4) is
// positive, else state 8.
void fn_710050c258(UnkGrabSm_b61* self, f32 dt) {
    self->timer += dt;
    UnkActor* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310));
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(actor->field_0x220), fn_7100d0ebf8()))
        return;
    fn_710050c490(self);
    self->field_0x6bc -= dt;
    if (!(self->field_0x6bc <= 0.0f))
        return;
    auto* ai = static_cast<UnkGrabAI_b61*>(self->owner->field_0x240);
    auto* emmy = ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? reinterpret_cast<UnkEmmyAI_b61*>(ai) : nullptr;
    if (fn_7100552ee4(emmy) > 0.0f)
        RequestState_b61<13, 14>(&self->machine, 9);
    else
        RequestState_b61<13, 14>(&self->machine, 8);
}

// The effect restart of src/game/CEmmyGrabSamusAttack/CEmmyGrabSamusAttack_Tick_b54.cpp in red: the level
// (+0x6d4) drops by 0.8 per second, not below zero.
struct UnkRGB_710050f26c {
    f32 r;
    f32 g;
    f32 b;
};
struct UnkColorIn_710050f26c {
    UnkRGB_710050f26c rgb;
    f32 a;
};
struct UnkColor_710050f26c {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
};
struct UnkStruct_710050f26c;

struct UnkGrabFx_b61 {
    u8 pad_0x0[0x28];
    UnkSmMachine_b61 machine;
    u8 pad_0xd8[0x6b4 - 0xd8];
    f32 timer;
    u8 pad_0x6b8[4];
    f32 field_0x6bc;
    u8 pad_0x6c0[0xc];
    f32 field_0x6cc;
    f32 field_0x6d0;
    f32 level;
};

template <class T>
static inline __attribute__((always_inline)) const T* Addr_b61(const T& value) {
    return &value;
}

static inline __attribute__((always_inline)) void Fade_b61(UnkGrabFx_b61* self, f32 step) {
    f32 level = self->level - step * 0.8f;
    level = level < 0.0f ? 0.0f : level;
    self->level = level;
    fn_710050f26c(reinterpret_cast<UnkStruct_710050f26c*>(self), self->field_0x6d0,
                  Addr_b61(UnkColorIn_710050f26c{{0.8f, 0.0f, 0.0f}, level}), self->field_0x6cc,
                  Addr_b61(UnkColor_710050f26c{1.0f, 0.0f, 0.0f, level}), 0.2f);
}

void fn_710050ce60(UnkGrabFx_b61* self, f32 dt) {
    self->timer += dt;
    Fade_b61(self, dt);
    UnkActor* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310));
    if (fn_710117b348(static_cast<UnkAnimationComponent*>(actor->field_0x220), fn_7100d0ebf8())) {
        Fade_b61(self, dt);
        return;
    }
    fn_710050c490(reinterpret_cast<UnkGrabSm_b61*>(self));
    self->field_0x6bc -= dt;
    if (!(self->field_0x6bc <= 0.0f))
        return;
    RequestState_b61<13, 14>(&self->machine, 10);
}

// ---- CHydrogigaAIComponent (tools/tu_map.py): machine at +0x14ca8, states 0..6, "none" 7 ----
template <u32 Offset>
struct UnkSmOwner_b61 {
    u8 pad_0x0[Offset];
    UnkSmMachine_b61 machine;
};

void fn_71005c0314(UnkSmOwner_b61<0x14ca8>* self) {
    RequestStateAt_b61<6, 7>(&self->machine,
                             reinterpret_cast<UnkSmStatePtr_b61*>(reinterpret_cast<char*>(self) + 0x14ccc), 5);
}

struct UnkHydrogigaParams_b61 {
    u8 pad_0x0[0x558];
    f32 field_0x558;
    u8 pad_0x55c[0x10];
    f32 field_0x56c;
};
struct UnkHydrogigaState_b61 {
    UnkHydrogigaParams_b61* params;  // 0x149a0
    u8 pad_0x8[0x1c];
    f32 timer;  // 0x149c4
    u8 pad_0x28[8];
    f32 field_0x30;  // 0x149d0
};
struct UnkHydrogigaSm_b61 {
    u8 pad_0x0[0x149a0];
    UnkHydrogigaState_b61 state;
    u8 pad_0x149d8[0x14ca8 - 0x149d8];
    UnkSmMachine_b61 machine;
};

// Asks for state 4 once the level of the two linked parts (+0x149a8, +0x149b0; looked up by id with
// fn_71005c1008) is at most 0.2: fn_7100a68874 of the first flagged one (+0x44), else the first part's
// value at +0x178.
char* fn_71005c1008(u64);
f32 fn_7100a68874(char*);

struct UnkHydrogigaPartRef_b61 {
    u8 pad_0x0[0x18];
    u64 id;
};
struct UnkHydrogigaPartLink_b61 {
    u8 pad_0x0[0x10];
    UnkHydrogigaPartRef_b61* ref;
};
struct UnkHydrogigaParts_b61 {
    u8 pad_0x0[0x149a8];
    UnkHydrogigaPartLink_b61* parts[2];
    u8 pad_0x149b8[0x14ca8 - 0x149b8];
    UnkSmMachine_b61 machine;
};

void fn_71005c2380(UnkHydrogigaParts_b61* self) {
    char* a = fn_71005c1008(self->parts[0]->ref->id);
    char* b = fn_71005c1008(self->parts[1]->ref->id);
    f32 level;
    if (a[0x44] | b[0x44])
        level = fn_7100a68874(a[0x44] ? a : b);
    else
        level = *reinterpret_cast<f32*>(a + 0x178);
    if (level <= 0.2f)
        RequestStateAt_b61<6, 7, true>(&self->machine,
                                       reinterpret_cast<UnkSmStatePtr_b61*>(reinterpret_cast<char*>(self) + 0x14ccc), 4);
}

// Once the timer reaches its tunable (+0x558), or the value at +0x149d0 reaches +0x56c, clears that value
// and asks for state 6.
void fn_71005c29e8(UnkHydrogigaSm_b61* self, f32 dt) {
    UnkHydrogigaState_b61* state = &self->state;
    state->timer += dt;
    UnkHydrogigaParams_b61* params = state->params;
    if (state->timer >= params->field_0x558 || state->field_0x30 >= params->field_0x56c) {
        state->field_0x30 = 0.0f;
        RequestStateAt_b61<6, 7, true>(&self->machine,
                                       reinterpret_cast<UnkSmStatePtr_b61*>(reinterpret_cast<char*>(self) + 0x14ccc), 6);
    }
}

#define UNK_HYDROGIGA_REQUEST_B61(owner, id)                                                                  \
    RequestStateAt_b61<6, 7, true>(                                                                            \
        &(owner)->machine, reinterpret_cast<UnkSmStatePtr_b61*>(reinterpret_cast<char*>(owner) + 0x14ccc), id)

// ---- CHydrogigaTongueSwirlAttack (tools/tu_map.py) ----
// After the base call, unless the owner's CHydrogigaAIComponent (+0x240) is in state 5, resets it
// (fn_71005c023c with false) and asks it for state 6.
struct UnkHydrogigaAttack_b61;
void fn_71005b2c7c(UnkHydrogigaAttack_b61* self);
struct UnkHydrogigaAI_b61 {
    void** vtable;
    u8 pad_0x8[0x14ca8 - 0x8];
    UnkSmMachine_b61 machine;

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkHydrogigaAI_b61*)>(vtable[0])(this);
    }
};
void fn_71005c023c(UnkHydrogigaAI_b61* self, bool value);

struct UnkHydrogigaAttackSelf_b61 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_71005b7a3c(UnkHydrogigaAttackSelf_b61* self) {
    fn_71005b2c7c(reinterpret_cast<UnkHydrogigaAttack_b61*>(self));
    auto* ai = static_cast<UnkHydrogigaAI_b61*>(self->owner->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_71005ba098()))
        return;
    fn_71005c023c(ai, false);
    UnkSmState_b61* current = ai->machine.current;
    if (current && current->id == 5)
        return;
    UNK_HYDROGIGA_REQUEST_B61(ai, 6);
}

// The Hydrogiga AI's grab timer: once the timer reaches its tunable (+0x550), loads the hold time
// (+0x554) and asks for state 6 unless the target is already held (fn_71009e6f9c or fn_71009e6434 of the
// target's +0x208). Then refreshes the hold time when the value at +0x149d0 reaches +0x568, and unless the
// target is held, counts it up to the tunable (clamped at zero from below).
struct UnkHelper;

struct UnkHydrogigaParams2_b61 {
    u8 pad_0x0[0x550];
    f32 field_0x550;
    f32 field_0x554;
    u8 pad_0x558[0x10];
    f32 field_0x568;
};
struct UnkHydrogigaState2_b61 {
    UnkHydrogigaParams2_b61* params;  // 0x149a0
    u8 pad_0x8[0x1c];
    f32 timer;  // 0x149c4
    f32 hold;   // 0x149c8
    u8 pad_0x2c[4];
    f32 field_0x30;  // 0x149d0
};
struct UnkHydrogigaSm2_b61 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x149a0 - 0x38];
    UnkHydrogigaState2_b61 state;
    u8 pad_0x149d8[0x14ca8 - 0x149d8];
    UnkSmMachine_b61 machine;
};

static inline __attribute__((always_inline)) bool TargetHeld_b61(UnkHydrogigaSm2_b61* self) {
    UnkActor* target = fn_7100d7c990(self->actor);
    if (!target)
        return false;
    auto* helper = static_cast<UnkHelper*>(target->field_0x208);
    if (!helper)
        return false;
    return fn_71009e6f9c(helper) || fn_71009e6434(helper);
}

void fn_71005c27f8(UnkHydrogigaSm2_b61* self, f32 dt) {
    UnkHydrogigaState2_b61* state = &self->state;
    state->timer += dt;
    UnkHydrogigaParams2_b61* params = state->params;
    if (state->timer >= params->field_0x550) {
        f32* hold = &state->hold;
        *hold = params->field_0x554;
        if (!(*hold >= params->field_0x554) || TargetHeld_b61(self))
            UNK_HYDROGIGA_REQUEST_B61(self, 6);
    }
    if (state->field_0x30 >= state->params->field_0x568)
        state->hold = state->params->field_0x554;
    if (TargetHeld_b61(self))
        return;
    f32 hold = state->hold;
    if (!(hold >= 0.0f))
        state->hold = 0.0f;
    else if (hold < state->params->field_0x554)
        state->hold = hold + dt;
}

// Once the timer reaches its tunable (+0x550), asks for state 1 and makes two calls of fn_7100fdcf2c on
// the music manager with 0.2 (as src/game/CHydrogigaAIComponent/CHydrogigaAIComponent_Send_b59_2.cpp).
extern UnkMgr_0xf18* g_unk_71726bc560;

static inline void* MusicManager_b61() {
    if (!reinterpret_cast<void*&>(g_unk_71726bc560)) {
        void* m = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(m);
        reinterpret_cast<void*&>(g_unk_71726bc560) = m;
    }
    return reinterpret_cast<void*&>(g_unk_71726bc560);
}

void fn_71005c21c0(UnkHydrogigaSm2_b61* self, f32 dt) {
    UnkHydrogigaState2_b61* state = &self->state;
    state->timer += dt;
    if (!(state->timer >= state->params->field_0x550))
        return;
    UNK_HYDROGIGA_REQUEST_B61(self, 1);
    fn_7100fdcf2c(MusicManager_b61(), 0, 0, 0.2f);
    fn_7100fdcf2c(MusicManager_b61(), 1, 1, 0.2f);
}

// ---- CEmmyGrabSamusAttack, continued ----
// Grab struggle: with a positive duration (+0x6b8) restarts the effects in green with timings 2 and twice
// the remaining fraction; when the flag at +0x6b1 is set asks for state 7 unless the target has an
// "ignore melee hit" track; otherwise counts down (+0x6bc) and asks for state 7 or 9 by the E.M.M.I.
// tunable's flag at +0xf5.
struct UnkGrabFx3_b61 {
    u8 pad_0x0[0x28];
    UnkSmMachine_b61 machine;
    u8 pad_0xd8[0x6b1 - 0xd8];
    bool field_0x6b1;
    u8 pad_0x6b2[2];
    f32 timer;
    f32 duration;
    f32 field_0x6bc;
    u8 pad_0x6c0[0xc];
    f32 field_0x6cc;
    f32 field_0x6d0;
    f32 level;
};

void fn_710050ca38(UnkGrabFx3_b61* self, f32 dt) {
    self->timer += dt;
    if (self->duration > 0.0f) {
        f32 fraction = self->field_0x6bc / self->duration;
        self->field_0x6d0 = 2.0f;
        self->level = 0.9f;
        self->field_0x6cc = fraction * 2.0f;
        fn_710050f26c(reinterpret_cast<UnkStruct_710050f26c*>(self), self->field_0x6d0,
                      Addr_b61(UnkColorIn_710050f26c{{0.0f, 0.3f, 0.0f}, self->level}), self->field_0x6cc,
                      Addr_b61(UnkColor_710050f26c{0.0f, 0.5f, 0.0f, self->level}), 0.2f);
    }
    if (self->field_0x6b1) {
        UnkActor* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310));
        if (!fn_710117b348(static_cast<UnkAnimationComponent*>(actor->field_0x220), fn_7100d0e6e8()))
            RequestState_b61<13, 14>(&self->machine, 7);
        return;
    }
    self->field_0x6bc -= dt;
    if (!(self->field_0x6bc <= 0.0f))
        return;
    if (reinterpret_cast<const u8*>(&g_unk_71726e0aa8)[0xf5])
        RequestState_b61<13, 14>(&self->machine, 7);
    else
        RequestState_b61<13, 14>(&self->machine, 9);
}

// Grab struggle, second form: the same effects; with the flag at +0x6b1 set and either the flag at +0x790
// or no "ignore melee hit" track, calls fn_710050afd0 with g_unk_71726de5a0 and asks for state 13;
// otherwise, once the countdown is over and +0x790 is clear, asks for state 13 or 12 by the player's part
// flag (as fn_710050add4).
struct UnkGrabFx4_b61 {
    u8 pad_0x0[0x28];
    UnkSmMachine_b61 machine;
    u8 pad_0xd8[0x6b1 - 0xd8];
    bool field_0x6b1;
    u8 pad_0x6b2[2];
    f32 timer;
    f32 duration;
    f32 field_0x6bc;
    u8 pad_0x6c0[0xc];
    f32 field_0x6cc;
    f32 field_0x6d0;
    f32 level;
    u8 pad_0x6d8[0x790 - 0x6d8];
    bool field_0x790;
};
struct UnkGrabArg_b61;

void fn_710050e9e4(UnkGrabFx4_b61* self, f32 dt) {
    self->timer += dt;
    if (self->duration > 0.0f) {
        f32 fraction = self->field_0x6bc / self->duration;
        self->field_0x6d0 = 2.0f;
        self->level = 0.9f;
        self->field_0x6cc = fraction * 2.0f;
        fn_710050f26c(reinterpret_cast<UnkStruct_710050f26c*>(self), self->field_0x6d0,
                      Addr_b61(UnkColorIn_710050f26c{{0.0f, 0.3f, 0.0f}, self->level}), self->field_0x6cc,
                      Addr_b61(UnkColor_710050f26c{0.0f, 0.5f, 0.0f, self->level}), 0.2f);
    }
    if (self->field_0x6b1) {
        if (!self->field_0x790) {
            UnkActor* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310));
            if (fn_710117b348(static_cast<UnkAnimationComponent*>(actor->field_0x220), fn_7100d0e6e8()))
                return;
        }
        fn_710050afd0(self, &g_unk_71726de5a0);
        RequestState_b61<13, 14>(&self->machine, 13);
        return;
    }
    f32 left = self->field_0x6bc - dt;
    bool busy = self->field_0x790;
    self->field_0x6bc = left;
    if (self->field_0x6bc <= 0.0f && !busy) {
        auto* player = reinterpret_cast<UnkGrabPlayer_b61*>(fn_7100dec788(g_unk_7101d47310));
        if (player->field_0x1c0->field_0x5c)
            RequestState_b61<13, 14>(&self->machine, 13);
        else
            RequestState_b61<13, 14>(&self->machine, 12);
    }
}
