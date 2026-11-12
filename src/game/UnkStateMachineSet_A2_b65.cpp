// Callers of a component's small state machine, after src/game/UnkStateMachineSet_b60.cpp (fn_7100818ab8,
// fn_7100918a78, fn_71007de4f0, fn_710081c584, fn_710088ba98): the request stores the pending id, gives up
// past the last state or while busy, and otherwise switches state. The units differ (CAbility,
// CAbilityMultiLockon, CAbilityShinespark, CChozoCommanderKiGrabAttack, CChozoCommanderUltimateGrabAttack,
// CChozoWarriorXUltimateGrabAttack, CCooldownXBossLavaDropsAttack, CAutoolGoToRepairPointAction); the
// template is repeated here under this file's own names, with the states-array parameter of
// src/game/UnkStateMachineSetA3_b61.cpp. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoWarriorXUltimateGrabAttack.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSmState_A2_b65 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A2_b65* __attribute__((aligned(4))) UnkSmStatePtr_A2_b65;

struct UnkSmMachine_A2_b65 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A2_b65* previous, UnkSmState_A2_b65* next);
    virtual void vf_0x28(UnkSmState_A2_b65* current, UnkSmState_A2_b65* next);

    u64 field_0x8;
    UnkSmState_A2_b65* current;
    UnkSmState_A2_b65* previous;
    u32 count;
    UnkSmStatePtr_A2_b65 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A2_b65 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A2_b65 machine;
};

template <bool Launder>
__attribute__((always_inline)) static inline bool FindState_A2_b65(UnkSmMachine_A2_b65* self, UnkSmStatePtr_A2_b65* states,
                                                               u32 id, UnkSmState_A2_b65*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A2_b65);
    asm("" : "+r"(n));
    UnkSmStatePtr_A2_b65* p = states ? states : self->states;
    if (Launder)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A2_b65)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 None, bool Launder = false>
__attribute__((always_inline)) static inline void SwitchState_A2_b65(UnkSmMachine_A2_b65* self, UnkSmStatePtr_A2_b65* states,
                                                                 u32 id, UnkSmState_A2_b65* current) {
    if (!current || current->id != id) {
        UnkSmState_A2_b65* found;
        UnkSmState_A2_b65* next;
        u32 enter;
        if (FindState_A2_b65<Launder>(self, states, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A2_b65*, s32);
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
__attribute__((always_inline)) static inline void RequestStateAt_A2_b65(UnkSmMachine_A2_b65* self,
                                                                    UnkSmStatePtr_A2_b65* states, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A2_b65<None, Launder>(self, states, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A2_b65(UnkSmMachine_A2_b65* self, u32 id) {
    RequestStateAt_A2_b65<Last, None>(self, nullptr, id);
}

#define UNK_SM_SET_CONST_A2_B65(addr, offset, id, last, none)       \
    void fn_##addr(UnkSmOwner_A2_b65<offset>* self) {                \
        RequestState_A2_b65<last, none>(&self->machine, id);         \
    }

UNK_SM_SET_CONST_A2_B65(710031a850, 0x3f0, 1, 7, 0x7fffffff)
UNK_SM_SET_CONST_A2_B65(71004330cc, 0x88, 1, 7, 0x7fffffff)
UNK_SM_SET_CONST_A2_B65(710032867c, 0x6a0, 1, 5, 6)
UNK_SM_SET_CONST_A2_B65(71004c7aa0, 0x68, 4, 4, 5)

// The same request after a base call (signatures copied from their existing declarations).
struct UnkAttackBase;
struct UnkSmLockon_A1_b64;
void fn_71003608ac(UnkAttackBase* self);

void fn_7100487484(UnkSmOwner_A2_b65<0x80>* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    RequestState_A2_b65<7, 0x7fffffff>(&self->machine, 2);
}

// `kind` (w1) reaches the ability's enter (fn_71002f9f18) untouched in the original (tools/forwarded_args.py).
void fn_710032984c(UnkSmOwner_A2_b65<0x6a0>* self, s32 kind) {
    fn_71002f9f18(reinterpret_cast<UnkSmLockon_A1_b64*>(self), kind);
    RequestState_A2_b65<5, 6>(&self->machine, 3);
}

struct UnkShinesparkTimer_A2_b65 {
    u8 pad_0x0[0x390];
    f32 timer;
    u8 pad_0x394[0x6a0 - 0x394];
    UnkSmMachine_A2_b65 machine;
};

void fn_710032e97c(UnkShinesparkTimer_A2_b65* self, f32 dt) {
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (!(self->timer <= 0.0f))
            return;
        RequestState_A2_b65<5, 6>(&self->machine, 1);
    }
}

struct UnkAbilityCharge_A2_b65 {
    u8 pad_0x0[0x10];
    UnkSmMachine_A2_b65 machine;
    u8 pad_0xc0[0xd4 - 0xc0];
    f32 limit;
    u8 pad_0xd8[0xe4 - 0xd8];
    f32 value;
};

void fn_71002f9aa0(UnkAbilityCharge_A2_b65* self) {
    if (!(self->value >= self->limit))
        return;
    RequestState_A2_b65<6, 7>(&self->machine, 6);
}

struct UnkGrabTimer_A2_b65 {
    u8 pad_0x0[0x80];
    UnkSmMachine_A2_b65 machine;
    u8 pad_0x130[0x138 - 0x130];
    f32 timer;
};

void fn_710048c144(UnkGrabTimer_A2_b65* self, f32 dt) {
    self->timer += dt;
    RequestState_A2_b65<7, 0x7fffffff>(&self->machine, 3);
}

struct UnkMsgTarget3_A2_b65 {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkMsgTarget2_A2_b65 {
    u8 pad_0x0[0x38];
    UnkMsgTarget3_A2_b65* field_0x38;
};
struct UnkMsgTarget_A2_b65 {
    u8 pad_0x0[0x10];
    UnkMsgTarget2_A2_b65* field_0x10;
};
struct UnkMsg_A2_b65 {
    s32 type;
    UnkMsgTarget_A2_b65* target;
};

bool fn_71004c8584(UnkSmOwner_A2_b65<0x68>* self, const UnkMsg_A2_b65* msg) {
    if (msg->type == 1 && msg->target->field_0x10->field_0x38->id == 0x667fbf1d7bb5877bull)
        RequestState_A2_b65<4, 5>(&self->machine, 1);
    return true;
}

// CAbilityShinespark: the request, then one timeline id removed from the actor (+0x30 of +0xc0).
struct UnkActor;
extern const u64 g_unk_7101648ea8;  // reached through the GOT

struct UnkShinesparkLink_A2_b65 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

struct UnkShinesparkSet_A2_b65 {
    u8 pad_0x0[0xc0];
    UnkShinesparkLink_A2_b65* link;
    u8 pad_0xc8[0x6a0 - 0xc8];
    UnkSmMachine_A2_b65 machine;
};

void fn_710032e220(UnkShinesparkSet_A2_b65* self) {
    RequestState_A2_b65<5, 6>(&self->machine, 1);
    fn_7100d7c94c(self->link->actor, &g_unk_7101648ea8);
}

// CChozoWarriorXUltimateGrabAttack: the request, then two calls on the owner's +0x220.
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);

struct UnkGrabOwner_A2_b65 {
    u8 pad_0x0[0x220];
    UnkActorTarget* field_0x220;
};

struct UnkGrabRelease_A2_b65 {
    u8 pad_0x0[0x8];
    UnkGrabOwner_A2_b65* owner;
    u8 pad_0x10[0x80 - 0x10];
    UnkSmMachine_A2_b65 machine;
};

void fn_7100489f3c(UnkGrabRelease_A2_b65* self) {
    RequestState_A2_b65<7, 0x7fffffff>(&self->machine, 1);
    UnkActorTarget* target = self->owner->field_0x220;
    fn_7101178e08(target, 5, false, false, -1.0f);
    fn_7101178e08(target, 4, false, false, -1.0f);
}

// CChozoCommanderKiGrabAttack: after the base call and a game-manager call, records whether the owner's
// animation (+0x220) has g_unk_71726c7df8 (+0x13d), then asks for state 3.

struct UnkStruct_710117af10;
struct UnkCls_710117af10;
extern UnkCls_710117af10* g_unk_71726c7df8 __attribute__((visibility("hidden")));
struct UnkCommanderAttack_A2_b65;
void fn_710042692c(UnkCommanderAttack_A2_b65*);
void fn_7100dfee18(UnkGameManager*);
void fn_7100dfef0c(UnkGameManager*);

struct UnkKiGrabTarget_A2_b65 {
    virtual void vf_0x0();
    void vf_0x168(u32 on, f32 value) {
        reinterpret_cast<void (*)(UnkKiGrabTarget_A2_b65*, u32, f32)>(
            (*reinterpret_cast<void***>(this))[0x168 / 8])(this, on, value);
    }
    void vf_0x180(bool a, bool b) {
        reinterpret_cast<void (*)(UnkKiGrabTarget_A2_b65*, bool, bool)>(
            (*reinterpret_cast<void***>(this))[0x180 / 8])(this, a, b);
    }
};

struct UnkKiGrabAiHolder_A2_b65 {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* ai;
};

struct UnkKiGrabOwner_A2_b65 {
    u8 pad_0x0[0x1d0];
    UnkKiGrabTarget_A2_b65* field_0x1d0;
    u8 pad_0x1d8[0x220 - 0x1d8];
    UnkStruct_710117af10* animation;
    u8 pad_0x228[0x240 - 0x228];
    UnkKiGrabAiHolder_A2_b65* field_0x240;
};

struct UnkKiGrab_A2_b65 {
    u8 pad_0x0[0x8];
    UnkKiGrabOwner_A2_b65* owner;
    u8 pad_0x10[0x88 - 0x10];
    UnkSmMachine_A2_b65 machine;
    u8 pad_0x138[0x13a - 0x138];
    bool field_0x13a;
    u8 pad_0x13b[0x13d - 0x13b];
    bool field_0x13d;
};

void fn_710043095c(UnkKiGrab_A2_b65* self) {
    fn_710042692c(reinterpret_cast<UnkCommanderAttack_A2_b65*>(self));
    fn_7100dfee18(g_unk_7101d47310);
    self->field_0x13d = false;
    UnkStruct_710117af10* animation = self->owner->animation;
    if (animation && fn_710117af10(animation, &g_unk_71726c7df8, false))
        self->field_0x13d = true;
    RequestState_A2_b65<7, 0x7fffffff>(&self->machine, 3);
}

// CAbility: when fn_71002f92d4 says so, or +0x47c is positive and +0x454 at least 1, moves state 3
// (pending, else current) on to 4.
struct UnkAbilityStep_A2_b65 {
    u8 pad_0x0[0x10];
    UnkSmMachine_A2_b65 machine;
    u8 pad_0xc0[0x454 - 0xc0];
    s32 field_0x454;
    u8 pad_0x458[0x47c - 0x458];
    f32 field_0x47c;
};
bool fn_71002f92d4(UnkAbilityStep_A2_b65*);

void fn_710030cf20(UnkAbilityStep_A2_b65* self) {
    if (!fn_71002f92d4(self) && !(self->field_0x47c > 0.0f && self->field_0x454 >= 1))
        return;
    u32 state = self->machine.pending;
    if (state == 7) {
        UnkSmState_A2_b65* current = self->machine.current;
        if (!current)
            return;
        state = current->id;
    }
    if (state == 3)
        RequestState_A2_b65<6, 7>(&self->machine, 4);
}

// CChozoCommanderUltimateGrabAttack: after the base call, when the owner's AI (+0x10 of +0x240) is a
// CCharClassChozoCommanderAIComponent, passes its +0x15a8 to the owner's +0x1d0 (vf_0x168); then a
// game-manager call and state 3.
void fn_710043e880(UnkKiGrab_A2_b65* self) {
    fn_710042692c(reinterpret_cast<UnkCommanderAttack_A2_b65*>(self));
    if (UnkKiGrabAiHolder_A2_b65* holder = self->owner->field_0x240) {
        if (base::reflection::UnkReflObject* ai = holder->ai) {
            if (fn_710009d50c(ai->vf_0x0(), fn_710044d0c0())) {
                if (UnkKiGrabTarget_A2_b65* target = self->owner->field_0x1d0)
                    target->vf_0x168(true, *reinterpret_cast<f32*>(reinterpret_cast<char*>(ai) + 0x15a8));
            }
        }
    }
    fn_7100dfef0c(g_unk_7101d47310);
    RequestState_A2_b65<7, 0x7fffffff>(&self->machine, 3);
}

// CAutoolGoToRepairPointAction: after the base start, clears the two ids (+0x3a0, +0x3a8), unlinks
// itself (+0x380) from the target its weak handle (+0x398) points at and drops the handle, then asks
// for state 6.

struct UnkGoToRepair_A2_b65 {
    u8 pad_0x0[0x2d0];
    UnkSmMachine_A2_b65 machine;
    u8 link[0x18];
    UnkWeakHandle* handle;
    s64 field_0x3a0;
    s64 field_0x3a8;
};

void fn_71003c0be0(UnkGoToRepair_A2_b65* self) {
    fn_7100394bd4(self);
    self->field_0x3a8 = -1;
    self->field_0x3a0 = -1;
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->link);
        ResetRef(&self->handle);
    }
    RequestState_A2_b65<6, 7>(&self->machine, 6);
}

// The same unit's message handler: on message 1, drops the handle as above and asks for state 4.
bool fn_71003c15c8(UnkGoToRepair_A2_b65* self, const s32* message) {
    if (*message != 1)
        return true;
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->link);
        ResetRef(&self->handle);
    }
    RequestState_A2_b65<6, 7>(&self->machine, 4);
    return false;
}

// CChozoCommanderUltimateGrabAttack: asks for state 1; then, with +0x13a set, copies it to +0x15ed2 of the
// owner's CChozoCommanderAIComponent (+0x240) and calls the owner's +0x1d0 (vf_0x180); otherwise
// passes the AI's +0x15a8 on as fn_710043e880 does.
void fn_7100440f5c(UnkKiGrab_A2_b65* self) {
    RequestState_A2_b65<7, 0x7fffffff>(&self->machine, 1);
    bool grabbed = self->field_0x13a;
    UnkKiGrabAiHolder_A2_b65* holder = self->owner->field_0x240;
    if (grabbed) {
        base::reflection::UnkReflObject* component = reinterpret_cast<base::reflection::UnkReflObject*>(holder);
        char* cast = fn_710009d50c(component->vf_0x0(), fn_710044e708()) ? reinterpret_cast<char*>(component)
                                                                          : nullptr;
        cast[0x15ed2] = self->field_0x13a;
        self->owner->field_0x1d0->vf_0x180(true, false);
        return;
    }
    if (holder) {
        if (base::reflection::UnkReflObject* ai = holder->ai) {
            if (fn_710009d50c(ai->vf_0x0(), fn_710044d0c0())) {
                if (UnkKiGrabTarget_A2_b65* target = self->owner->field_0x1d0)
                    target->vf_0x168(true, *reinterpret_cast<f32*>(reinterpret_cast<char*>(ai) + 0x15a8));
            }
        }
    }
}
