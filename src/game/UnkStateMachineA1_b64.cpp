// Users of a component's small state machine, one per unit (tools/tu_map.py names each below): updates (the
// template of src/game/UnkStateMachineUpdateA5_b62.cpp, copied: applies the pending state, then updates the
// current state and applies what it asked for, at most ten times, while marked busy) and requests (the
// template of src/game/UnkStateMachineSet_b60.cpp). A machine far into its owner (+0x14978) reads its states
// array from the owner (UnkSmStatesAt_A1_b64), as the original does. Names are placeholders.
#include "game/CAbility.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CCharClassCentralUnitCannonAIComponent.h"
#include "game/CCharClassChozoWarriorXUltimateGrabAttack.h"
#include "game/CChozoWarriorXAIComponent.h"
#include "game/CChozoWarriorXUltimateGrabAttack.h"
#include "game/CFXComponent.h"
#include "game/CGunComponent.h"
#include "game/CInventoryComponent.h"
#include "game/CLifeComponent.h"
#include "game/CSamusGunComponent.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU437.h"
#include "types.h"
#include <math.h>
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "game_s2/UnkWeakHandle.h"
#include "game/UnkGameManager.h"
#include "game/UnkAnimationComponent.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkGameBlackboard.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTimelineComponent.h"

struct UnkSmState_A1_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A1_b64* __attribute__((aligned(4))) UnkSmStatePtr_A1_b64;

struct UnkSmMachine_A1_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A1_b64* previous, UnkSmState_A1_b64* next);
    virtual void vf_0x28(UnkSmState_A1_b64* current, UnkSmState_A1_b64* next);

    u64 field_0x8;
    UnkSmState_A1_b64* current;
    UnkSmState_A1_b64* previous;
    u32 count;
    UnkSmStatePtr_A1_b64 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A1_b64 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A1_b64 machine;
};

// Where a far machine's states are read from: the owner plus an offset (else the machine's own array).
struct UnkSmStatesAt_A1_b64 {
    char* owner = nullptr;
    u32 offset = 0;
};

__attribute__((always_inline)) static inline bool FindState_A1_b64(UnkSmMachine_A1_b64* self, UnkSmStatesAt_A1_b64 states,
                                                               u32 id, UnkSmState_A1_b64*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A1_b64);
    asm("" : "+r"(n));
    UnkSmStatePtr_A1_b64* p = states.offset
        ? reinterpret_cast<UnkSmStatePtr_A1_b64*>(states.owner + states.offset) : self->states;
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A1_b64)) {
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
__attribute__((always_inline)) static inline void SwitchState_A1_b64(UnkSmMachine_A1_b64* self, UnkSmStatesAt_A1_b64 states, u32 id,
                                                                 UnkSmState_A1_b64* current) {
    if (!current || current->id != id) {
        UnkSmState_A1_b64* found;
        UnkSmState_A1_b64* next;
        u32 enter;
        if (FindState_A1_b64(self, states, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A1_b64*, s32);
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
__attribute__((always_inline)) static inline void Apply_A1_b64(UnkSmMachine_A1_b64* self,
                                                           UnkSmStatesAt_A1_b64 states = UnkSmStatesAt_A1_b64()) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_A1_b64<None>(self, states, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_A1_b64(UnkSmMachine_A1_b64* self, f32 dt,
                                                            UnkSmStatesAt_A1_b64 states = UnkSmStatesAt_A1_b64()) {
    self->busy = true;
    if (self->pending != None)
        Apply_A1_b64<Last, None>(self, states);
    u32 i = 0;
    while (UnkSmState_A1_b64* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_A1_b64<Last, None>(self, states);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

// ---- Requests (the b60 request template on the same machine) ----
template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void RequestState_A1_b64(
    UnkSmMachine_A1_b64* self, u32 id, UnkSmStatesAt_A1_b64 states = UnkSmStatesAt_A1_b64()) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_A1_b64<None>(self, states, id, self->current);
}

// CCentralUnitCannonAIComponent's unit (machine +0x14978): stores the argument, then true in states 2-5;
// in states 1 and 6, requests state 2.
struct UnkSmCannon_A1_b64 {
    u8 pad_0x0[0x146cc];
    u32 field_0x146cc;
    u8 pad_0x146d0[0x14978 - 0x146d0];
    UnkSmMachine_A1_b64 machine;
};

bool fn_71003fc0a8(UnkSmCannon_A1_b64* self, u32 value) {
    self->field_0x146cc = value;
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return false;
    u32 id = current->id;
    if (id - 2 < 4)
        return true;
    if (id == 6 || id == 1) {
        char* owner = reinterpret_cast<char*>(self);
        asm("" : "+r"(owner));
        RequestState_A1_b64<6, 0x7fffffff>(&self->machine, 2, UnkSmStatesAt_A1_b64{owner, 0x1499c});
        return true;
    }
    return false;
}

struct UnkInventory;

// CAbility's unit (machine +0x10, states 0-6, none 7): requests driven by two virtual checks.
struct UnkSmAbilityActor_A1_b64 {
    u8 pad_0x0[0x270];
    UnkInventory* field_0x270;
    u8 pad_0x278[0x3d2 - 0x278];
    bool field_0x3d2;
};

struct UnkSmAbilityFlags_A1_b64 {
    u8 pad_0x0[0x30];
    UnkSmAbilityActor_A1_b64* actor;
    u8 pad_0x38[0xd2 - 0x38];
    bool flag;
};

struct UnkSmAbilityId_A1_b64 {
    u8 pad_0x0[0x20];
    UnkQualifiedId id;
};

struct UnkSmAbility_A1_b64 {
    virtual void vf_0x0();
    u8 pad_0x8[0x10 - 0x8];
    UnkSmMachine_A1_b64 machine;
    UnkSmAbilityFlags_A1_b64* field_0xc0;
    UnkSmAbilityId_A1_b64* field_0xc8;
    u8 pad_0xd0[0xd4 - 0xd0];
    f32 field_0xd4;
    u8 pad_0xd8[0xdc - 0xd8];
    bool field_0xdc;
    bool field_0xdd;
};

typedef bool (*UnkSmCheckFn_A1_b64)(UnkSmAbility_A1_b64*);

__attribute__((always_inline)) static inline bool Check_A1_b64(UnkSmAbility_A1_b64* self, u32 offset) {
    return (*reinterpret_cast<UnkSmCheckFn_A1_b64**>(self))[offset / 8](self);
}

__attribute__((always_inline)) static inline u32 State_A1_b64(UnkSmMachine_A1_b64* self, u32 none) {
    if (self->pending != none)
        return self->pending;
    return self->current ? self->current->id : none;
}

__attribute__((always_inline)) static inline bool IsState_A1_b64(UnkSmMachine_A1_b64* self, u32 none, u32 id) {
    if (self->pending != none)
        return self->pending == id;
    return self->current && self->current->id == id;
}

void fn_71002f8be0(UnkSmAbility_A1_b64* self, bool force) {
    if (!force && State_A1_b64(&self->machine, 7) - 5 < 2)
        return;
    if (self->field_0xd4 > 0.0f)
        RequestState_A1_b64<6, 7>(&self->machine, 5);
    else
        RequestState_A1_b64<6, 7>(&self->machine, 6);
}

void fn_71002f8a18(UnkSmAbility_A1_b64* self) {
    if (Check_A1_b64(self, 0x190)) {
        if (self->field_0xdd) {
            fn_71002f8be0(self, false);
            return;
        }
        RequestState_A1_b64<6, 7>(&self->machine, 4);
    } else {
        RequestState_A1_b64<6, 7>(&self->machine, 3);
    }
}

void fn_71002f8848(UnkSmAbility_A1_b64* self) {
    if (Check_A1_b64(self, 0x120)) {
        if (self->field_0xdc && self->field_0xc0->flag) {
            fn_71002f8a18(self);
            return;
        }
        RequestState_A1_b64<6, 7>(&self->machine, 2);
    } else {
        RequestState_A1_b64<6, 7>(&self->machine, 1);
    }
}

void fn_71002f9460(UnkSmAbility_A1_b64* self, bool on) {
    if (self->field_0xdd == on)
        return;
    self->field_0xdd = on;
    if (on && State_A1_b64(&self->machine, 7) == 4 && Check_A1_b64(self, 0x190)) {
        fn_71002f8be0(self, false);
        return;
    }
    if (self->field_0xdd || State_A1_b64(&self->machine, 7) < 5)
        return;
    if (Check_A1_b64(self, 0x190))
        RequestState_A1_b64<6, 7>(&self->machine, 4);
    else
        RequestState_A1_b64<6, 7>(&self->machine, 3);
}

void fn_71002f9694(UnkSmAbility_A1_b64* self, bool on) {
    bool active = State_A1_b64(&self->machine, 7) > 1;
    if (!(active ^ on))
        return;
    UnkSmAbilityActor_A1_b64* actor = self->field_0xc0->actor;
    if (UnkInventory* inventory = actor->field_0x270) {
        actor->field_0x3d2 = false;
        fn_7101142a28(inventory, self->field_0xc8->id, true, true, true, on ? 1.0f : 0.0f, 0.0f);
        actor->field_0x3d2 = true;
    }
    if (on)
        fn_71002f8848(self);
    else
        RequestState_A1_b64<6, 7>(&self->machine, 1);
}


// CAbilityShinespark's unit (machine +0x6a0, states 0-5, none 6): the owner's facing feeds two steps; a
// flag drops once the facing leaves the allowed cone, and state 4 is requested when the timer runs out.
struct UnkAimTunable {
    u8 pad_0x0[0xb8];
    f32 field_0xb8;
};

struct UnkSmVec3_A1_b64 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkSmFacing_A1_b64 {
    u8 pad_0x0[0x9ec];
    UnkSmVec3_A1_b64 dir;
};

struct UnkSmActor_A1_b64 {
    u8 pad_0x0[0x1d8];
    UnkSmFacing_A1_b64* facing;
};

struct UnkSmOwnerLink_A1_b64 {
    u8 pad_0x0[0x30];
    UnkSmActor_A1_b64* actor;
};

struct UnkSmShinespark_A1_b64 {
    u8 pad_0x0[0xc0];
    UnkSmOwnerLink_A1_b64* owner;
    u8 pad_0xc8[0x394 - 0xc8];
    f32 timer;
    u8 pad_0x398[0x39c - 0x398];
    bool flag;
    u8 pad_0x39d[0x6a0 - 0x39d];
    UnkSmMachine_A1_b64 machine;
};

void fn_710032f4cc(UnkSmShinespark_A1_b64*, const UnkSmVec3_A1_b64&);
void fn_710032f35c(UnkSmShinespark_A1_b64*, const UnkSmVec3_A1_b64&, f32);

void fn_710032f810(UnkSmShinespark_A1_b64* self, f32 dt) {
    const UnkSmVec3_A1_b64& facing = self->owner->actor->facing->dir;
    UnkSmVec3_A1_b64 dir = {facing.x, facing.y, facing.z};
    f32 angle = atan2f(dir.y, fabsf(dir.x));
    if (self->flag) {
        f32 limit = g_unk_71726b1438.field_0xb8;
        if (dir.z <= 0.2f || angle > limit)
            self->flag = false;
    }
    fn_710032f4cc(self, dir);
    fn_710032f35c(self, dir, dt);
    self->timer -= dt;
    if (self->timer <= 0.0f)
        RequestState_A1_b64<5, 6>(&self->machine, 4);
}

// CAbilitySpeedBooster's unit (CAbility's machine +0x10; its own at +0x3a8, states 0-3, none 4): in the
// base state 6, a timeline key by the owner's flag; then by its own state, state 3 is requested once the
// owner's mover stops, or the next step runs while it still moves.
struct UnkUnit435Self;
extern const u64 g_unk_7101648ff8;
extern const u64 g_unk_7101649000;

struct UnkSmFlagged_A1_b64 {
    u8 pad_0x0[0x1dc];
    f32 speed;
    u8 pad_0x1e0[0x3a8 - 0x1e0];
    bool flag;
};

struct UnkSmBoosterActor_A1_b64 {
    u8 pad_0x0[0x1f8];
    UnkSmFlagged_A1_b64* field_0x1f8;
    u8 pad_0x200[0x210 - 0x200];
    UnkSmFlagged_A1_b64* mover;
};

struct UnkSmBoosterLink_A1_b64 {
    u8 pad_0x0[0x30];
    UnkSmBoosterActor_A1_b64* actor;
};

struct UnkSmBooster_A1_b64 {
    u8 pad_0x0[0x10];
    UnkSmMachine_A1_b64 base_machine;
    UnkSmBoosterLink_A1_b64* owner;
    u8 pad_0xc8[0x3a8 - 0xc8];
    UnkSmMachine_A1_b64 machine;
};

void fn_710033dcdc(UnkSmBooster_A1_b64* self) {
    UnkSmBoosterActor_A1_b64* actor = self->owner->actor;
    if (State_A1_b64(&self->base_machine, 7) == 6 && actor->field_0x1f8)
        fn_7100d7c828(reinterpret_cast<UnkActor*>(actor),
                      actor->field_0x1f8->flag ? &g_unk_7101648ff8 : &g_unk_7101649000, false);
    UnkSmFlagged_A1_b64* mover = actor ? actor->mover : nullptr;
    UnkSmState_A1_b64* current = self->machine.current;
    u32 id = current ? current->id : 4;
    if (!mover) {
        if (id == 3)
            fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
    } else if (id == 2) {
        if (!mover->flag && mover->speed <= 0.0f)
            RequestState_A1_b64<3, 4>(&self->machine, 3);
    } else if (current && current->id == 3 && (mover->flag || mover->speed > 0.0f)) {
        fn_710033cd50(reinterpret_cast<UnkUnit435Self*>(self), 1);
    }
}

// CCentralUnitCannonAIComponent's unit again: the AI object's class test (its result unused, as in
// fn_71003fd0b0), then a timer that counts up to the tunable at +0x30 before state 3 is requested.
struct UnkTunable;

struct UnkSmCannonObject_A1_b64 {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkSmCannon2_A1_b64 {
    u8 pad_0x0[0x10];
    UnkSmCannonObject_A1_b64* object;
    u8 pad_0x18[0x146c8 - 0x18];
    f32 timer;
    u8 pad_0x146cc[0x14978 - 0x146cc];
    UnkSmMachine_A1_b64 machine;
};

void fn_71003fd76c(UnkSmCannon2_A1_b64* self, f32 dt) {
    f32 timer = self->timer;
    if (self->object)
        fn_710009d50c(self->object->vf_0x0(), fn_71003feb44());
    if (timer < *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71726c32a0) + 0x30)) {
        self->timer += dt;
        return;
    }
    char* owner = reinterpret_cast<char*>(self);
    asm("" : "+r"(owner));
    RequestState_A1_b64<6, 0x7fffffff>(&self->machine, 3, UnkSmStatesAt_A1_b64{owner, 0x1499c});
}

// The same unit's message handler: a kind-1 message whose sender's id (four links in) names one of two
// events moves the cannon on: in states 1-2 to state 3, in state 6 back to state 1.
struct UnkSmMsgId_A1_b64 {
    u8 pad_0x0[0x30];
    u64 id;
};
struct UnkSmMsgInfo_A1_b64 {
    u8 pad_0x0[0x38];
    UnkSmMsgId_A1_b64* info;
};
struct UnkSmMsgSender_A1_b64 {
    u8 pad_0x0[0x10];
    UnkSmMsgInfo_A1_b64* sender;
};
struct UnkSmMsg_A1_b64 {
    u32 kind;
    UnkSmMsgSender_A1_b64* data;
};

bool fn_71003fcdc0(UnkSmOwner_A1_b64<0x14978>* self, UnkSmMsg_A1_b64* msg) {
    if (msg->kind == 1) {
        if (UnkSmState_A1_b64* current = self->machine.current) {
            char* owner = reinterpret_cast<char*>(self);
            asm("" : "+r"(owner));
            UnkSmMsgInfo_A1_b64* sender = msg->data->sender;
            switch (current->id) {
            case 1:
            case 2:
                if (sender->info->id == 0xa0e33be014f44bd7)
                    RequestState_A1_b64<6, 0x7fffffff>(&self->machine, 3, UnkSmStatesAt_A1_b64{owner, 0x1499c});
                break;
            case 6:
                if (sender->info->id == 0x6a17390fc912b900)
                    RequestState_A1_b64<6, 0x7fffffff>(&self->machine, 1, UnkSmStatesAt_A1_b64{owner, 0x1499c});
                break;
            }
        }
    }
    return true;
}

// CChozoCommanderKiGrabAttack's unit (machine +0x88, states 0-12, none 0x7fffffff), as the ultimate grab
// attacks of src/game/UnkStateMachineUpdateA2_b63.cpp.
template <u32 Offset>
struct UnkSmGrab_A1_b64 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A1_b64 machine;
    bool done;
    bool blocked;
    u8 pad_after_blocked[2];
    bool requested;  // set with the request of state 7
};

// Requests one of two states by the done flag.
void fn_7100432ce0(UnkSmGrab_A1_b64<0x88>* self) {
    if (self->done)
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 7);
    else
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 6);
}

// On a kind-1 message of the 0xf4061ff25a2163d6 kind, states 6 and 7 go back to state 1. Always handled.
struct UnkSmGrabMsgInfo_A1_b64 {
    u8 pad_0x0[0x30];
    u64 hash;
};
struct UnkSmGrabMsg_A1_b64 {
    u32 kind;
    u8 pad_0x4[0xc];
    UnkSmGrabMsgInfo_A1_b64* info;
};

bool fn_7100432918(UnkSmGrab_A1_b64<0x88>* self, UnkSmGrabMsg_A1_b64* msg) {
    if (msg->info->hash != 0xf4061ff25a2163d6 || msg->kind != 1)
        return true;
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return true;
    switch (current->id) {
    case 6:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 1);
        break;
    case 7:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 1);
        break;
    }
    return true;
}

// The same message in states 3-7: states 3 and 5 go on to 4, state 4 picks its next state by the done
// flag (fn_7100432ce0), states 6 and 7 go back to 1. Always handled.
bool fn_7100432a98(UnkSmGrab_A1_b64<0x88>* self, UnkSmGrabMsg_A1_b64* msg) {
    if (msg->info->hash != 0xf4061ff25a2163d6 || msg->kind != 1)
        return true;
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return true;
    switch (current->id) {
    case 3:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 4);
        break;
    case 4:
        fn_7100432ce0(self);
        break;
    case 5:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 4);
        break;
    case 6:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 1);
        break;
    case 7:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 1);
        break;
    }
    return true;
}

// By the current state: in state 3 the grab goes on to 5 unless the player is in the first track; in state 4
// it is done unless blocked or the player is in it; in state 6 the second track requests state 7.
struct UnkOwner;

struct UnkSmPlayer_A1_b64 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

__attribute__((always_inline)) static inline bool InTrack_A1_b64(const base::reflection::CClass* (*cls)()) {
    UnkSmPlayer_A1_b64* player = reinterpret_cast<UnkSmPlayer_A1_b64*>(fn_7100defcd4(g_unk_7101d47310));
    return fn_710117b348(player->animation, cls());
}

void fn_7100432e7c(UnkSmGrab_A1_b64<0x88>* self) {
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return;
    switch (current->id) {
    case 3:
        if (InTrack_A1_b64(fn_7100d0e6e8))
            return;
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 5);
        return;
    case 4:
        if (self->blocked || InTrack_A1_b64(fn_7100d0e6e8))
            return;
        self->done = true;
        return;
    case 6:
        if (self->blocked)
            return;
        if (!InTrack_A1_b64(fn_7100d19258))
            return;
        self->requested = true;
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 7);
        return;
    }
}

// CCooldownXBossLavaDropsAttack's unit (machine +0x68, states 0-4, none 5): a timer, the AI component (when
// it is of the unit's AI class) steps the target position, then by the tunables state 2 or 3 is requested.
struct UnkWorld;
struct UnkPart;

struct UnkSmLavaAI_A1_b64 {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkSmLavaTunable_A1_b64 {
    u8 pad_0x0[0x100];
    f32 field_0x100;
    f32 field_0x104;
    f32 field_0x108;
};

struct UnkSmLavaSink_A1_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(UnkSmVec3_A1_b64* position);
};

struct UnkSmLava_A1_b64 {
    u8 pad_0x0[0x8];
    UnkActor* actor;
    u8 pad_0x10[0x68 - 0x10];
    UnkSmMachine_A1_b64 machine;
    u8 pad_0x118_[0x118 - 0x118];
    UnkSmLavaTunable_A1_b64* tunable;
    u8 pad_0x120[0x2e0 - 0x120];
    f32 time;
    f32 near_time;
    u8 pad_0x2e8[0x2f0 - 0x2e8];
    UnkSmVec3_A1_b64 position;
    u8 pad_0x2fc[0x320 - 0x2fc];
    UnkSmLavaSink_A1_b64* sink;
};

void fn_71004d24d0(UnkSmLavaAI_A1_b64*, UnkSmVec3_A1_b64*, f32);

__attribute__((always_inline)) static inline UnkSmLavaAI_A1_b64* LavaAI_A1_b64(UnkActor* actor) {
    UnkSmLavaAI_A1_b64* ai = static_cast<UnkSmLavaAI_A1_b64*>(actor->field_0x240);
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), fn_71004cf318()) ? ai : nullptr;
}

void fn_71004c7ba4(UnkSmLava_A1_b64* self, f32 dt) {
    self->time += dt;
    fn_71004d24d0(LavaAI_A1_b64(self->actor), &self->position, dt);
    if (self->time >= self->tunable->field_0x100)
        RequestState_A1_b64<4, 5>(&self->machine, 2);
}

void fn_71004c7ee4(UnkSmLava_A1_b64* self, f32 dt) {
    self->time += dt;
    UnkSmLavaAI_A1_b64* ai = LavaAI_A1_b64(self->actor);
    fn_71004d24d0(ai, &self->position, dt);
    if (self->time >= self->tunable->field_0x104) {
        if (UnkActor* target = fn_7100d7c990(self->actor)) {
            UnkSmVec3_A1_b64 t = {target->field_0x44, target->field_0x54, target->field_0x64};
            const UnkSmVec3_A1_b64& p = self->position;
            if ((p.x - t.x) * (p.x - t.x) + (p.y - t.y) * (p.y - t.y) + (p.z - t.z) * (p.z - t.z) < 100.0f) {
                self->near_time += dt;
                if (self->near_time >= self->tunable->field_0x108)
                    RequestState_A1_b64<4, 5>(&self->machine, 3);
            } else {
                self->near_time = 0.0f;
            }
        }
    }
    UnkWorld* world = *reinterpret_cast<UnkWorld**>(reinterpret_cast<char*>(self->actor) + 0x280);
    UnkSmLavaSink_A1_b64* part = reinterpret_cast<UnkSmLavaSink_A1_b64*>(fn_7101152b90(world, 0x5659b769f8e44087));
    part->vf_0x10(&self->position);
    self->sink->vf_0x10(&self->position);
    typedef void (*AimFn)(UnkSmLavaAI_A1_b64*, UnkSmVec3_A1_b64*);
    (*reinterpret_cast<AimFn**>(ai))[0x838 / 8](ai, &self->position);
}

// behaviortree::CAutoolGoToRepairPointAction's unit (machine +0x2d0, states 0-6, none 7): once the AI
// component (of the unit's AI class) arrives and its flag is clear, state 3.
struct UnkSmAutoolAI_A1_b64 {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkSmAutool_A1_b64 {
    u8 pad_0x0[0x18];
    UnkActor* actor;
    u8 pad_0x20[0x2d0 - 0x20];
    UnkSmMachine_A1_b64 machine;
};

bool fn_71003c6c1c(UnkSmAutoolAI_A1_b64*, f32);

void fn_71003c1ca8(UnkSmAutool_A1_b64* self, f32 dt) {
    UnkSmAutoolAI_A1_b64* ai = static_cast<UnkSmAutoolAI_A1_b64*>(self->actor->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71003c2a94()))
        ai = nullptr;
    if (fn_71003c6c1c(ai, dt) && !reinterpret_cast<bool*>(ai)[0x14771])
        RequestState_A1_b64<6, 7>(&self->machine, 3);
}

// CChozoCommanderUltimateGrabAttack's message handler: on a kind-1 message of the 0xf4061ff25a2163d6 kind,
// by the current state (machine +0x88) its step or the next state. Always handled. The steps of states 4
// and 9 are defined in src/game/UnkStateMachineUpdateA2_b63.cpp.
template <u32 Offset>
struct UnkSmGrab_A2_b63;
void fn_7100440770(UnkSmGrab_A2_b63<0x88>* self);
void fn_7100440a38(UnkSmGrab_A2_b63<0x88>* self);
void fn_7100440644(UnkSmGrab_A1_b64<0x88>*, UnkSmGrabMsg_A1_b64* msg);
void fn_710044090c(UnkSmGrab_A1_b64<0x88>*, UnkSmGrabMsg_A1_b64* msg);
void fn_7100440f5c(UnkSmGrab_A1_b64<0x88>*);

bool fn_7100440418(UnkSmGrab_A1_b64<0x88>* self, UnkSmGrabMsg_A1_b64* msg) {
    if (msg->info->hash != 0xf4061ff25a2163d6 || msg->kind != 1)
        return true;
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return true;
    switch (current->id) {
    case 6:
    case 11:
    case 12:
        fn_7100440f5c(self);
        break;
    case 3:
        fn_7100440644(self, msg);
        break;
    case 4:
        fn_7100440770(reinterpret_cast<UnkSmGrab_A2_b63<0x88>*>(self));
        break;
    case 5:
        fn_710044090c(self, msg);
        break;
    case 7:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 8);
        break;
    case 8:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 9);
        break;
    case 9:
        fn_7100440a38(reinterpret_cast<UnkSmGrab_A2_b63<0x88>*>(self));
        break;
    case 10:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 9);
        break;
    }
    return true;
}

// CChozoWarriorXUltimateGrabAttack's message handler, the same over its machine at +0x80; states 6, 11 and 12
// run fn_7100489ea0's step (inlined here).
void fn_71004896b4(UnkSmGrab_A2_b63<0x80>* self);
void fn_710048997c(UnkSmGrab_A2_b63<0x80>* self);
void fn_7100489588(UnkSmGrab_A1_b64<0x80>*, UnkSmGrabMsg_A1_b64* msg);
void fn_7100489850(UnkSmGrab_A1_b64<0x80>*, UnkSmGrabMsg_A1_b64* msg);
struct UnkAIComponent;
struct UnkSelf;

struct UnkSmWarriorAI_A1_b64 {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkSmWarriorOwner_A1_b64 {
    u8 pad_0x0[0x1d0];
    void* field_0x1d0;
    u8 pad_0x1d8[0x240 - 0x1d8];
    UnkSmWarriorAI_A1_b64* component;
};

__attribute__((always_inline)) static inline void WarriorStep_A1_b64(UnkSmGrab_A1_b64<0x80>* self) {
    fn_7100489f3c(reinterpret_cast<UnkSelf*>(self));
    UnkSmWarriorOwner_A1_b64** owner = reinterpret_cast<UnkSmWarriorOwner_A1_b64**>(reinterpret_cast<char*>(self) + 8);
    if (reinterpret_cast<u8*>(self)[0x132]) {
        UnkSmWarriorAI_A1_b64* component = (*owner)->component;
        if (component) {
            UnkSmWarriorAI_A1_b64* found =
                fn_710009d50c(component->vf_0x0(), fn_710048db44()) ? component : nullptr;
            fn_710048fc80(reinterpret_cast<UnkAIComponent*>(found));
        } else {
            fn_710048fc80(nullptr);
        }
    } else if ((*owner)->field_0x1d0) {
        fn_7101160348((*owner)->field_0x1d0,
                      *reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_71726ce7d0) + 0x2c));
    }
}

bool fn_71004892f0(UnkSmGrab_A1_b64<0x80>* self, UnkSmGrabMsg_A1_b64* msg) {
    if (msg->info->hash != 0xf4061ff25a2163d6 || msg->kind != 1)
        return true;
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return true;
    switch (current->id) {
    case 6:
    case 11:
    case 12:
        WarriorStep_A1_b64(self);
        break;
    case 3:
        fn_7100489588(self, msg);
        break;
    case 4:
        fn_71004896b4(reinterpret_cast<UnkSmGrab_A2_b63<0x80>*>(self));
        break;
    case 5:
        fn_7100489850(self, msg);
        break;
    case 7:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 8);
        break;
    case 8:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 9);
        break;
    case 9:
        fn_710048997c(reinterpret_cast<UnkSmGrab_A2_b63<0x80>*>(self));
        break;
    case 10:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 9);
        break;
    }
    return true;
}

// The Warrior's other handler of the same message: states 6, 11 and 12 run the step, state 7 requests 8.
bool fn_7100489130(UnkSmGrab_A1_b64<0x80>* self, UnkSmGrabMsg_A1_b64* msg) {
    if (msg->info->hash != 0xf4061ff25a2163d6 || msg->kind != 1)
        return true;
    UnkSmState_A1_b64* current = self->machine.current;
    if (!current)
        return true;
    switch (current->id) {
    case 6:
        WarriorStep_A1_b64(self);
        break;
    case 7:
        RequestState_A1_b64<12, 0x7fffffff>(&self->machine, 8);
        break;
    case 11:
        WarriorStep_A1_b64(self);
        break;
    case 12:
        WarriorStep_A1_b64(self);
        break;
    }
    return true;
}

// CAbilityMultiLockon's unit (machine +0x3f0, none 0x7fffffff): the start, after the base step: state 2,
// the flags cleared, the gun component's current gun name kept, the lock-on gun selected (the hidden id
// g_unk_71726af978), the gun told when it is of its class, then the owner's object at +0x208 stepped.
struct UnkSamusGunComponent;
struct UnkComponent;
struct UnkSamusGun_b58;
const base::global::CStrId& fn_7100840844(UnkSamusGunComponent*);
using base::global::CStrId;
void fn_710083f32c(UnkSamusGunComponent* self, const CStrId* name, bool a, bool b, bool c);
extern const base::global::CStrId g_unk_71726af978 __attribute__((visibility("hidden")));

struct UnkSmGunClass_A1_b64 {
    virtual const base::reflection::CClass* vf_0x0();
};

struct UnkSmLockonOwner_A1_b64 {
    u8 pad_0x0[0x208];
    void* field_0x208;
    u8 pad_0x210[0x2a8 - 0x210];
    UnkSamusGunComponent* gun;
};

// A lock-on target: two weak handles (the owner's actor and the target) and the target's id.
__attribute__((always_inline)) static inline UnkWeakTarget* Get_A1_b64(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

__attribute__((always_inline)) static inline void Drop_A1_b64(UnkWeakHandle* handle) {
    if (handle && --handle->ref_count == 0)
        handle->vf_0x10();
}

struct UnkSmLockonEntry_A1_b64 {
    UnkWeakHandle* owner;
    UnkWeakHandle* target;
    u8 pad_0x10[0x18 - 0x10];
    u64 id;
    u8 pad_0x20[0x30 - 0x20];
    ~UnkSmLockonEntry_A1_b64() {
        Drop_A1_b64(target);
        Drop_A1_b64(owner);
    }
};

struct UnkSmLockon_A1_b64 {
    u8 pad_0x0[0x390];
    u32 field_0x390;
    u8 pad_0x394[0x3f0 - 0x394];
    UnkSmMachine_A1_b64 machine;
    u8 pad_0x4a0[0x5f0 - 0x4a0];
    u16 field_0x5f0;
    u8 pad_0x5f2[0x5f8 - 0x5f2];
    base::global::CStrId previous;
    UnkSmLockonOwner_A1_b64* owner;
    u8 pad_0x608[0x60c - 0x608];
    u32 count;
    UnkSmLockonEntry_A1_b64 entries[1];
};

// `kind` (w1) reaches the ability's enter (fn_71002f9f18) untouched in the original (tools/forwarded_args.py).
void fn_7100319cb8(UnkSmLockon_A1_b64* self, s32 kind) {
    fn_71002f9f18(self, kind);
    RequestState_A1_b64<7, 0x7fffffff>(&self->machine, 2);
    self->field_0x5f0 = 0;
    UnkSamusGunComponent* gun = self->owner->gun;
    self->previous = fn_7100840844(gun);
    fn_710083f32c(gun, &g_unk_71726af978, false, true, false);
    if (gun) {
        UnkSmGunClass_A1_b64* object = reinterpret_cast<UnkSmGunClass_A1_b64*>(gun);
        if (fn_710009d50c(object->vf_0x0(), fn_7100857564()))
            fn_7100859938(reinterpret_cast<UnkSamusGun_b58*>(gun));
    }
    if (void* object = self->owner->field_0x208)
        fn_71009eaa88(object);
}

// The same unit: unless the gun component holds the lock-on gun, state 1; otherwise state 1 when its
// controller (a CLockOnMissileGun) is not busy, then the next step.
bool fn_710084088c(UnkSamusGunComponent*);
void fn_710031c208(UnkSmLockon_A1_b64*);

void fn_710031c530(UnkSmLockon_A1_b64* self) {
    UnkSamusGunComponent* gun = self->owner->gun;
    if (!fn_710084088c(gun) || fn_7100840844(gun).data != g_unk_71726af978.data) {
        RequestState_A1_b64<7, 0x7fffffff>(&self->machine, 1);
        return;
    }
    UnkSmGunClass_A1_b64* ctrl =
        static_cast<UnkSmGunClass_A1_b64*>(fn_710083cd30(reinterpret_cast<UnkComponent*>(gun)));
    if (ctrl && fn_710009d50c(ctrl->vf_0x0(), fn_710084bb24())) {
        typedef bool (*BusyFn)(UnkSmGunClass_A1_b64*);
        if (!(*reinterpret_cast<BusyFn**>(ctrl))[0x60 / 8](ctrl))
            RequestState_A1_b64<7, 0x7fffffff>(&self->machine, 1);
    }
    fn_710031c208(self);
}

// The same unit: the stop, after the base step: state 1, the gun kept at the start selected again (and
// told), each lock-on target released (fn_710031a13c) and its owner's AI told the target, then the
// targets dropped.
struct UnkSmLockonAI_A1_b64 {
    virtual const base::reflection::CClass* vf_0x0();
};
struct UnkSmLockonActor_A1_b64 {
    u8 pad_0x0[0x230];
    void* field_0x230;
    u8 pad_0x238[0x2a0 - 0x238];
    UnkSmLockonAI_A1_b64* ai;
};

void fn_710031a13c(UnkSmLockon_A1_b64*, u64, bool);
void fn_7100725610(UnkSmLockonAI_A1_b64*, UnkWeakTarget*);

void fn_7100319e98(UnkSmLockon_A1_b64* self) {
    fn_71002fa068(self);
    RequestState_A1_b64<7, 0x7fffffff>(&self->machine, 1);
    UnkSamusGunComponent* gun = self->owner->gun;
    fn_710083f32c(gun, &self->previous, false, true, false);
    if (gun) {
        UnkSmGunClass_A1_b64* object = reinterpret_cast<UnkSmGunClass_A1_b64*>(gun);
        if (fn_710009d50c(object->vf_0x0(), fn_7100857564()))
            fn_7100859938(reinterpret_cast<UnkSamusGun_b58*>(gun));
    }
    u64 size = (u64)self->count * sizeof(UnkSmLockonEntry_A1_b64);
    asm("" : "+r"(size));
    if (size != 0) {
        UnkSmLockonEntry_A1_b64* end = self->entries + self->count;
        UnkSmLockonEntry_A1_b64* e = self->entries;
        do {
            fn_710031a13c(self, e->id, false);
            if (Get_A1_b64(e->target) && Get_A1_b64(e->owner)) {
                if (UnkSmLockonAI_A1_b64* ai =
                        reinterpret_cast<UnkSmLockonActor_A1_b64*>(Get_A1_b64(e->owner))->ai) {
                    UnkSmLockonAI_A1_b64* found =
                        fn_710009d50c(ai->vf_0x0(), fn_7100723914()) ? ai : nullptr;
                    UnkSmLockonActor_A1_b64* actor = reinterpret_cast<UnkSmLockonActor_A1_b64*>(e->owner->target);
                    asm volatile("" : "+r"(found));
                    if (actor->field_0x230)
                        fn_7100725610(found, Get_A1_b64(e->target));
                }
            }
            ++e;
        } while (e != end);
    }
    for (u32 i = 0; i < self->count; ++i)
        self->entries[i].~UnkSmLockonEntry_A1_b64();
    self->count = 0;
    self->field_0x390 = 0;
}
