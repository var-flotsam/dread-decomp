// Users of a component's small state machine, one per unit (tools/tu_map.py names each below): updates (the
// template of src/game/UnkStateMachineUpdateA5_b62.cpp, copied: applies the pending state, then updates the
// current state and applies what it asked for, at most ten times, while marked busy) and requests (the
// template of src/game/UnkStateMachineSet_b60.cpp). A machine far into its owner (+0x14978) reads its states
// array from the owner (UnkSmStatesAt_A2_b63), as the original does. Names are placeholders.
#include "game/CAbility.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CChozoWarriorAttack.h"
#include "game/UnkTU2702.h"
#include "types.h"
#include "game_s2/UnkSpitAttack.h"
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkGameManager.h"
#include "game/UnkAnimationComponent.h"
#include "base/reflection/UnkClassGetters.h"

struct UnkSmState_A2_b63 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A2_b63* __attribute__((aligned(4))) UnkSmStatePtr_A2_b63;

struct UnkSmMachine_A2_b63 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A2_b63* previous, UnkSmState_A2_b63* next);
    virtual void vf_0x28(UnkSmState_A2_b63* current, UnkSmState_A2_b63* next);

    u64 field_0x8;
    UnkSmState_A2_b63* current;
    UnkSmState_A2_b63* previous;
    u32 count;
    UnkSmStatePtr_A2_b63 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A2_b63 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A2_b63 machine;
};

// Where a far machine's states are read from: the owner plus an offset (else the machine's own array).
struct UnkSmStatesAt_A2_b63 {
    char* owner = nullptr;
    u32 offset = 0;
};

__attribute__((always_inline)) static inline bool FindState_A2_b63(UnkSmMachine_A2_b63* self, UnkSmStatesAt_A2_b63 states,
                                                               u32 id, UnkSmState_A2_b63*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A2_b63);
    asm("" : "+r"(n));
    UnkSmStatePtr_A2_b63* p = states.offset
        ? reinterpret_cast<UnkSmStatePtr_A2_b63*>(states.owner + states.offset) : self->states;
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A2_b63)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

// Switches to state `id` (the b59 "apply pending" body) and clears the pending id.
template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_A2_b63(UnkSmMachine_A2_b63* self, UnkSmStatesAt_A2_b63 states, u32 id,
                                                                 UnkSmState_A2_b63* current) {
    if (!current || current->id != id) {
        UnkSmState_A2_b63* found;
        UnkSmState_A2_b63* next;
        u32 enter;
        if (FindState_A2_b63(self, states, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A2_b63*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Apply_A2_b63(UnkSmMachine_A2_b63* self,
                                                           UnkSmStatesAt_A2_b63 states = UnkSmStatesAt_A2_b63()) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_A2_b63<None>(self, states, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_A2_b63(UnkSmMachine_A2_b63* self, f32 dt,
                                                            UnkSmStatesAt_A2_b63 states = UnkSmStatesAt_A2_b63()) {
    self->busy = true;
    if (self->pending != None)
        Apply_A2_b63<Last, None>(self, states);
    u32 i = 0;
    while (UnkSmState_A2_b63* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_A2_b63<Last, None>(self, states);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

// CAbility's unit: two timers around the update of a machine at +0x10 (states 0-6, none 7).
struct UnkSmAbility_A2_b63 {
    u8 pad_0x0[0x10];
    UnkSmMachine_A2_b63 machine;
    u8 pad_0xc0[0xd8 - 0xc0];
    f32 time_0xd8;
    u8 pad_0xdc[0xe4 - 0xdc];
    f32 time_0xe4;
};

void fn_71002f8de4(UnkSmAbility_A2_b63* self, f32 dt) {
    self->time_0xd8 += dt;
    Update_A2_b63<6, 7>(&self->machine, dt);
    self->time_0xe4 += dt;
}

// A base update, then the time scaled by the owner's rate (when positive), then the machine's update.
struct UnkSmRate_A2_b63;
f32 fn_7100d7ce40(UnkSmRate_A2_b63*);
void fn_7100426c70(void*, f32 dt);  // the base update, with the step (s0)

template <u32 Offset>
struct UnkSmRateOwner_A2_b63 {
    u8 pad_0x0[0x8];
    UnkSmRate_A2_b63* rate;
    u8 pad_0x10[Offset - 0x10];
    UnkSmMachine_A2_b63 machine;
};

#define UNK_SM_UPDATE_RATE_A2_B63(addr, base, offset, last, none)         \
    void fn_##addr(UnkSmRateOwner_A2_b63<offset>* self, f32 dt) {         \
        base;                                                             \
        UnkSmMachine_A2_b63* machine = &self->machine;                    \
        UnkSmRate_A2_b63* rate = self->rate;                              \
        asm("" : : "r"(machine));                                         \
        if (fn_7100d7ce40(rate) > 0.0f)                                   \
            dt /= fn_7100d7ce40(self->rate);                              \
        Update_A2_b63<last, none>(machine, dt);                           \
    }

UNK_SM_UPDATE_RATE_A2_B63(7100430df0, fn_7100426c70(self, dt), 0x88, 7, 0x7fffffff)
UNK_SM_UPDATE_RATE_A2_B63(710043eba4, fn_7100426c70(self, dt), 0x88, 12, 0x7fffffff)
UNK_SM_UPDATE_RATE_A2_B63(710048770c, fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt), 0x80, 12, 0x7fffffff)

// CCentralUnitCannonAIComponent's unit: nothing while the game manager's check holds, else the base update
// and the machine's (far into the owner).
struct UnkSelf_7100369c44;

void fn_71003fc438(UnkSmOwner_A2_b63<0x14978>* self, f32 dt) {
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    UnkSmMachine_A2_b63* machine = &self->machine;
    asm("" : "+r"(machine));
    fn_71011000e0(reinterpret_cast<UnkSelf_7100369c44*>(self), dt);
    Update_A2_b63<6, 0x7fffffff>(machine, dt,
                                 UnkSmStatesAt_A2_b63{reinterpret_cast<char*>(self), 0x1499c});
}

// An ability built on CAbility's update (fn_71002f8de4): from state 2 on, a cooldown that a check can cut
// short, the ability's own step, and a second machine (+0x3a8, states 0-3, none 4); then the last step.
struct UnkSmAbility2_A2_b63 {
    u8 pad_0x0[0x10];
    UnkSmMachine_A2_b63 base_machine;
    void* field_0xc0;
    u8 pad_0xc8[0x39c - 0xc8];
    f32 cooldown;
    u8 pad_0x3a0;
    bool cut;
    u8 pad_0x3a2[0x3a8 - 0x3a2];
    UnkSmMachine_A2_b63 machine;
};

void* fn_7100300bd0(void*, u32);
void fn_710033bc80(UnkSmAbility2_A2_b63*, f32);
void fn_710033bf64(UnkSmAbility2_A2_b63*, f32);

__attribute__((always_inline)) static inline u32 State_A2_b63(UnkSmMachine_A2_b63* self, u32 none) {
    if (self->pending != none)
        return self->pending;
    return self->current ? self->current->id : none;
}

void fn_710033b99c(UnkSmAbility2_A2_b63* self, f32 dt) {
    fn_71002f8de4(reinterpret_cast<UnkSmAbility_A2_b63*>(self), dt);
    if (State_A2_b63(&self->base_machine, 7) >= 2) {
        if (self->cooldown > 0.0f) {
            if (fn_710033c174(self)) {
                self->cut = true;
                self->cooldown = 0.0f;
            } else {
                self->cooldown -= dt;
                if (self->cooldown <= 0.0f) {
                    self->cooldown = 0.0f;
                    fn_7100300bd0(self->field_0xc0, 0);
                }
            }
        }
        fn_710033bc80(self, dt);
        Update_A2_b63<3, 4>(&self->machine, dt);
    }
    fn_710033bf64(self, dt);
}

// Another ability on CAbility's update: in states 1-2 of its own machine (+0x6a0, states 0-5, none 6),
// a check of the owner's track (CCharClassCheckShinesparkAllowingActivationTrack) and facing that triggers
// another ability; then the base update, from state 2 on a cooldown a virtual check can cut short, and the
// machine's update; then the last step.
struct UnkAbility;
struct UnkAbilitySet;

struct UnkSmFacing_A2_b63 {
    u8 pad_0x0[0x9ec];
    f32 x;
    f32 y;
};
struct UnkSmFlagged_A2_b63 {
    u8 pad_0x0[0x3a8];
    bool flag;
};
struct UnkSmActor_A2_b63 {
    u8 pad_0x0[0x1d8];
    UnkSmFacing_A2_b63* facing;
    u8 pad_0x1e0[0x208 - 0x1e0];
    UnkSmFlagged_A2_b63* field_0x208;
    u8 pad_0x210[0x220 - 0x210];
    UnkAnimationComponent* animation;
};
struct UnkSmOwnerLink_A2_b63 {
    u8 pad_0x0[0x30];
    UnkSmActor_A2_b63* actor;
};
struct UnkSmAbility3_A2_b63 {
    u8 pad_0x0[0x10];
    UnkSmMachine_A2_b63 base_machine;
    UnkSmOwnerLink_A2_b63* owner;
    u8 pad_0xc8[0x3f8 - 0xc8];
    f32 cooldown;
    u8 pad_0x3fc[0x6a0 - 0x3fc];
    UnkSmMachine_A2_b63 machine;
};

void fn_7100328b3c(UnkSmAbility3_A2_b63*);
void fn_7100328e68(UnkSmAbility3_A2_b63*, f32);

void fn_7100328784(UnkSmAbility3_A2_b63* self, f32 dt) {
    UnkSmState_A2_b63* current = self->machine.current;
    if (current && (u32)(current->id - 1) <= 1) {
        UnkSmActor_A2_b63* actor = self->owner->actor;
        if (actor && fn_710117b348(actor->animation, fn_7100c0fe98()) && actor->field_0x208 &&
            actor->field_0x208->flag) {
            UnkSmFacing_A2_b63* facing = actor->facing;
            f32 x = facing->x;
            f32 y = facing->y;
            if (x * 0.0f + y * -1.0f >= 0.70710677f) {
                UnkAbility* other =
                    fn_71003012f8(reinterpret_cast<UnkAbilitySet*>(self->owner), fn_710033b6a4());
                if (other) {
                    typedef void (*ActivateFn)(UnkAbility*, u32);
                    (*reinterpret_cast<ActivateFn**>(other))[0xa0 / 8](other, 1);
                }
                fn_7100328b3c(self);
            }
        }
    }
    fn_71002f8de4(reinterpret_cast<UnkSmAbility_A2_b63*>(self), dt);
    if (State_A2_b63(&self->base_machine, 7) >= 2) {
        if (self->cooldown > 0.0f) {
            typedef bool (*CheckFn)(UnkSmAbility3_A2_b63*);
            if ((*reinterpret_cast<CheckFn**>(self))[0xa8 / 8](self)) {
                self->cooldown = 0.0f;
                fn_71002fa070(self);
            } else {
                self->cooldown -= dt;
                if (self->cooldown <= 0.0f)
                    self->cooldown = 0.0f;
            }
        }
        Update_A2_b63<5, 6>(&self->machine, dt);
        UnkSmState_A2_b63* now = self->machine.current;
        if (!now || now->id != 1)
            fn_7100328e68(self, dt);
    }
}

// ---- Requests (the b60 request template on the same machine) ----
template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A2_b63(
    UnkSmMachine_A2_b63* self, u32 id, UnkSmStatesAt_A2_b63 states = UnkSmStatesAt_A2_b63()) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A2_b63<None>(self, states, id, self->current);
}

// The ultimate grab attacks (CChozoCommanderUltimateGrabAttack, CChozoWarriorXUltimateGrabAttack): by the
// current state (machine at +0x88), whether the player is in one of two tracks moves the grab on.
struct UnkOwner;
UnkOwner* fn_7100defcd4(UnkGameManager*);
s32 fn_7100dffc58(UnkGameManager*);

template <u32 Offset>
struct UnkSmGrab_A2_b63 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A2_b63 machine;
    bool done;
    bool blocked;
    u8 pad_after_blocked[3];
    bool requested;  // set with the requests of states 7 and 12
};

__attribute__((always_inline)) static inline bool InTrack_A2_b63(const base::reflection::CClass* (*cls)()) {
    UnkSmActor_A2_b63* player = reinterpret_cast<UnkSmActor_A2_b63*>(fn_7100defcd4(g_unk_7101d47310));
    return fn_710117b348(player->animation, cls());
}

// TrackFirst: states 4 and 9 test the track before the blocked flag.
template <u32 Offset, bool TrackFirst>
__attribute__((always_inline)) static inline void Grab_A2_b63(UnkSmGrab_A2_b63<Offset>* self) {
    UnkSmState_A2_b63* current = self->machine.current;
    if (!current)
        return;
    switch (current->id) {
    case 3:
        if (InTrack_A2_b63(fn_7100d0e6e8))
            return;
        if (fn_7100dffc58(g_unk_7101d47310) == 3) {
            self->done = true;
            return;
        }
        RequestState_A2_b63<12, 0x7fffffff>(&self->machine, 5);
        return;
    case 4:
    case 9:
        if (TrackFirst) {
            if (InTrack_A2_b63(fn_7100d0e6e8) || self->blocked)
                return;
        } else {
            if (self->blocked || InTrack_A2_b63(fn_7100d0e6e8))
                return;
        }
        self->done = true;
        return;
    case 6:
        if (self->blocked)
            return;
        if (!InTrack_A2_b63(fn_7100d19258))
            return;
        self->requested = true;
        RequestState_A2_b63<12, 0x7fffffff>(&self->machine, 7);
        return;
    case 8:
        if (InTrack_A2_b63(fn_7100d0e6e8))
            return;
        if (fn_7100dffc58(g_unk_7101d47310) == 3) {
            self->done = true;
            return;
        }
        RequestState_A2_b63<12, 0x7fffffff>(&self->machine, 10);
        return;
    case 11:
        if (self->blocked)
            return;
        if (!InTrack_A2_b63(fn_7100d19258))
            return;
        self->requested = true;
        RequestState_A2_b63<12, 0x7fffffff>(&self->machine, 12);
        return;
    }
}

void fn_7100440bd4(UnkSmGrab_A2_b63<0x88>* self) {
    Grab_A2_b63<0x88, false>(self);
}

void fn_7100489b18(UnkSmGrab_A2_b63<0x80>* self) {
    Grab_A2_b63<0x80, true>(self);
}

// The same attacks: request one of two states by the done flag.
template <u32 Offset, u32 IfDone, u32 Otherwise>
__attribute__((always_inline)) static inline void GrabNext_A2_b63(UnkSmGrab_A2_b63<Offset>* self) {
    if (self->done)
        RequestState_A2_b63<12, 0x7fffffff>(&self->machine, IfDone);
    else
        RequestState_A2_b63<12, 0x7fffffff>(&self->machine, Otherwise);
}

void fn_7100440770(UnkSmGrab_A2_b63<0x88>* self) {
    GrabNext_A2_b63<0x88, 7, 6>(self);
}

void fn_7100440a38(UnkSmGrab_A2_b63<0x88>* self) {
    GrabNext_A2_b63<0x88, 12, 11>(self);
}

void fn_71004896b4(UnkSmGrab_A2_b63<0x80>* self) {
    GrabNext_A2_b63<0x80, 7, 6>(self);
}

void fn_710048997c(UnkSmGrab_A2_b63<0x80>* self) {
    GrabNext_A2_b63<0x80, 12, 11>(self);
}

// Attack starts: a base step, (the lazily created manager,) state 1, then two links dropped from the targets
// of their weak handles.
template <u32 Offset>
struct UnkSmLinked_A2_b63 {
    u8 pad_0x0[Offset - 0x40];
    u8 link_a[0x18];
    UnkWeakHandle* handle_a;
    u8 link_b[0x18];
    UnkWeakHandle* handle_b;
    UnkSmMachine_A2_b63 machine;
};

__attribute__((always_inline)) static inline void Unlink_A2_b63(UnkWeakHandle** slot, void* link) {
    if (UnkWeakHandle* handle = *slot) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        ResetRef(slot);
    }
}

template <u32 Offset>
__attribute__((always_inline)) static inline void StartLinked_A2_b63(UnkSmLinked_A2_b63<Offset>* self) {
    RequestState_A2_b63<12, 0x7fffffff>(&self->machine, 1);
    Unlink_A2_b63(&self->handle_a, self->link_a);
    Unlink_A2_b63(&self->handle_b, self->link_b);
}

void fn_71004269a4(UnkSmLinked_A2_b63<0x88>*);
void fn_7100da928c(UnkManager_71726af468* manager);

void fn_710043e9f4(UnkSmLinked_A2_b63<0x88>* self) {
    fn_71004269a4(self);
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    fn_7100da928c(manager);
    StartLinked_A2_b63(self);
}

struct UnkSelf;

void fn_710048758c(UnkSmLinked_A2_b63<0x80>* self) {
    fn_71004936a0(reinterpret_cast<UnkSelf*>(self));
    StartLinked_A2_b63(self);
}

// CCentralUnitCannonAIComponent's unit again (machine +0x14978): in states 3-5, request state 4.
bool fn_71003fc314(UnkSmOwner_A2_b63<0x14978>* self) {
    UnkSmState_A2_b63* current = self->machine.current;
    if (current && (u32)(current->id - 3) <= 2) {
        char* owner = reinterpret_cast<char*>(self);
        asm("" : "+r"(owner));
        RequestState_A2_b63<6, 0x7fffffff>(&self->machine, 4, UnkSmStatesAt_A2_b63{owner, 0x1499c});
        return true;
    }
    return false;
}
