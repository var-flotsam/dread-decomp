// Component updates around the small state machine update (template copied from
// src/game/UnkStateMachineUpdateA5_b62.cpp, fn_7100886c10): applies the pending state, then updates the
// current state and applies what it asked for, at most ten times, while marked busy. Names are placeholders.
#include "game/CUsableComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1450.h"
#include "game/UnkTU2538.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2656.h"
#include "game/UnkTU2737.h"
#include "types.h"

struct UnkSmState_A6_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    virtual void vf_0x20(f32 dt);        // updated
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_A6_b64* __attribute__((aligned(4))) UnkSmStatePtr_A6_b64;

struct UnkSmMachine_A6_b64 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_A6_b64* previous, UnkSmState_A6_b64* next);
    virtual void vf_0x28(UnkSmState_A6_b64* current, UnkSmState_A6_b64* next);

    u64 field_0x8;
    UnkSmState_A6_b64* current;
    UnkSmState_A6_b64* previous;
    u32 count;
    UnkSmStatePtr_A6_b64 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_A6_b64 {
    u8 pad_0x0[Offset];
    UnkSmMachine_A6_b64 machine;
};

// Where a far machine's states are read from: the owner plus an offset (else the machine's own array;
// lever from src/game/UnkStateMachineUpdateA2_b63.cpp).
struct UnkSmStatesAt_A6_b64 {
    char* owner = nullptr;
    u32 offset = 0;
};

__attribute__((always_inline)) static inline bool FindState_A6_b64(UnkSmMachine_A6_b64* self, UnkSmStatesAt_A6_b64 states,
                                                               u32 id, UnkSmState_A6_b64*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_A6_b64);
    asm("" : "+r"(n));
    UnkSmStatePtr_A6_b64* p = states.offset
        ? reinterpret_cast<UnkSmStatePtr_A6_b64*>(states.owner + states.offset) : self->states;
    if (states.offset)
        asm("" : "+r"(p));
    for (; n != 0; ++p, n -= sizeof(UnkSmStatePtr_A6_b64)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 None>
__attribute__((always_inline)) static inline void SwitchState_A6_b64(UnkSmMachine_A6_b64* self, UnkSmStatesAt_A6_b64 states, u32 id,
                                                                 UnkSmState_A6_b64* current) {
    if (!current || current->id != id) {
        UnkSmState_A6_b64* found;
        UnkSmState_A6_b64* next;
        u32 enter;
        if (FindState_A6_b64(self, states, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_A6_b64*, s32);
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
__attribute__((always_inline)) static inline void Apply_A6_b64(UnkSmMachine_A6_b64* self,
                                                           UnkSmStatesAt_A6_b64 states = UnkSmStatesAt_A6_b64()) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_A6_b64<None>(self, states, id, self->current);
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void Update_A6_b64(UnkSmMachine_A6_b64* self, f32 dt) {
    self->busy = true;
    if (self->pending != None)
        Apply_A6_b64<Last, None>(self);
    u32 i = 0;
    while (UnkSmState_A6_b64* current = self->current) {
        current->vf_0x20(dt);
        if (self->pending == None)
            break;
        Apply_A6_b64<Last, None>(self);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

template <u32 Last, u32 None, class Arg>
__attribute__((always_inline)) static inline void UpdateWith_A6_b64(UnkSmMachine_A6_b64* self, Arg arg,
                                                                UnkSmStatesAt_A6_b64 states = UnkSmStatesAt_A6_b64()) {
    typedef void (*UpdatedFn)(UnkSmState_A6_b64*, Arg);
    self->busy = true;
    if (self->pending != None)
        Apply_A6_b64<Last, None>(self, states);
    u32 i = 0;
    while (UnkSmState_A6_b64* current = self->current) {
        (*reinterpret_cast<UpdatedFn**>(current))[4](current, arg);
        if (self->pending == None)
            break;
        Apply_A6_b64<Last, None>(self, states);
        if (i++ > 8)
            break;
    }
    self->busy = false;
}

// CSamusGrappleMovementState's unit: keeps the argument at +0xb88 while it runs.
void fn_71009b0d24(void* self, void* arg);

struct UnkGrappleOwner_A6_b64 : UnkSmOwner_A6_b64<0x188> {
    u8 pad_after[0xb88 - 0x188 - sizeof(UnkSmMachine_A6_b64)];
    void* arg;
};

void fn_710099a6b4(UnkGrappleOwner_A6_b64* self, void* arg) {
    self->arg = arg;
    fn_71009b0d24(self, arg);
    UpdateWith_A6_b64<4, 5, void*>(&self->machine, arg);
    self->arg = nullptr;
}

// GUI::CGlobalMapComposition's unit: only while enabled (+0xb4).
struct UnkGlobalMapOwner_A6_b64 {
    u8 pad_0x0[0xb4];
    bool enabled;
    u8 pad_0xb5[0x238 - 0xb5];
    UnkSmMachine_A6_b64 machine;
};

void fn_7100b29fbc(UnkGlobalMapOwner_A6_b64* self, f32 dt) {
    if (self->enabled)
        Update_A6_b64<6, 0x7fffffff>(&self->machine, dt);
}

// Passes a flag (+0x50) and dt to the lazily created object of unit 2727 (g_unk_71727bc260; layout as in
// src/game/UnkUnit2727/UnkUnit2727_c2_1.cpp), then updates.
#include "base/UnkVector.h"

struct UnkObj_71011a50cc {
    u8 unk_0x0[0x98];
    UnkObj_71011a50cc();  // fn_71011a50cc
};

struct UnkFlagOwner_A6_b64 {
    u8 pad_0x0[0x50];
    bool flag;
    u8 pad_0x51[7];
    UnkSmMachine_A6_b64 machine;
};

void fn_7100f1380c(UnkFlagOwner_A6_b64* self, f32 dt) {
    if (!g_unk_71727bc260)
        g_unk_71727bc260 = new (fn_71000839a0(0x98, 0x3f)) UnkObj_71011a50cc();
    fn_71011a5798(g_unk_71727bc260, self->flag, dt);
    Update_A6_b64<3, 0x7fffffff>(&self->machine, dt);
}

// CUsableComponent's unit: then, when a timeline restart is pending (+0x538) and the lazily created
// manager (g_unk_7101d47360) doesn't hold fn_7100eef674, clears it and plays one of two timelines on the
// actor at +0x30, picked by vf 0x3a8.
#include "game/UnkTimelineComponent.h"

struct UnkManager;
extern const u64 g_unk_710164b648[];
extern const u64 g_unk_710164b650[];

struct UnkUsableOwner_A6_b64 {
    virtual void vf_0x0();
    u8 pad_0x8[0x30 - 8];
    UnkActor* actor;
    u8 pad_0x38[0x100 - 0x38];
    UnkSmMachine_A6_b64 machine;
    u8 pad_after[0x538 - 0x100 - sizeof(UnkSmMachine_A6_b64)];
    bool restart;

    bool vf_0x3a8() { return (*reinterpret_cast<bool (***)(UnkUsableOwner_A6_b64*)>(this))[0x3a8 / 8](this); }
};

void fn_7100a9fad8(UnkUsableOwner_A6_b64* self, f32 dt) {
    Update_A6_b64<7, 8>(&self->machine, dt);
    if (!self->restart)
        return;
    if (!g_unk_7101d47360) {
        void* mgr = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(mgr);
        g_unk_7101d47360 = static_cast<UnkManager*>(mgr);
    }
    if (fn_7100eef674(g_unk_7101d47360))
        return;
    self->restart = false;
    if (self->vf_0x3a8())
        fn_7100d7c828(self->actor, g_unk_710164b648, false);
    else
        fn_7100d7c828(self->actor, g_unk_710164b650, false);
}


// Unit 1454 (next to unit 1452's owner getter fn_710097f1b8, declared as in
// src/game/UnkUnit1452/UnkUnit1452_Send_b56.cpp): clears the flag at +0x3f0 when either of the owner's two
// values (+0x1d8 -> +0x9f0/+0x9f4) is out of range, then updates the machine with a pointer argument.
struct UnkUnit1452Owner_b56;
struct UnkUnit1452_b56;

struct UnkUnit1454Values_A6_b64 {
    u8 pad_0x0[0x9f0];
    f32 value_0x9f0;
    f32 value_0x9f4;
};

struct UnkUnit1454Owner_A6_b64 {
    u8 pad_0x0[0x1d8];
    UnkUnit1454Values_A6_b64* values;
};

struct UnkUnit1454_A6_b64 {
    u8 pad_0x0[0x30];
    UnkSmMachine_A6_b64 machine;
    u8 pad_after[0x3f0 - 0x30 - sizeof(UnkSmMachine_A6_b64)];
    bool flag;
};

void fn_7100985b88(UnkUnit1454_A6_b64* self, void* arg) {
    if (self->flag) {
        UnkUnit1454Values_A6_b64* values = reinterpret_cast<UnkUnit1454Owner_A6_b64*>(
            fn_710097f1b8(reinterpret_cast<UnkUnit1452_b56*>(self)))->values;
        if ((values->value_0x9f0 < 0.0f) | (values->value_0x9f4 < 0.2f))
            self->flag = false;
    }
    UpdateWith_A6_b64<3, 4, void*>(&self->machine, arg);
}

// Unit 1734: then, when a return to the main menu is pending (+0x552) and the game manager's effect
// player (+0x1040) doesn't veto it, switches the game manager to "MAINMENU" and clears it.
#include "base/global/CRntString.h"
#include "game/UnkGameManager.h"

using base::global::CRntString;

struct UnkUnit1734_A6_b64 {
    u8 pad_0x0[0x50];
    UnkSmMachine_A6_b64 machine;
    u8 pad_after[0x552 - 0x50 - sizeof(UnkSmMachine_A6_b64)];
    bool to_main_menu;
};

void fn_7100b91c38(UnkUnit1734_A6_b64* self, f32 dt) {
    Update_A6_b64<3, 4>(&self->machine, dt);
    if (self->to_main_menu && !fn_7101053a98(g_unk_7101d47310->field_0x1040)) {
        fn_7100deac9c(g_unk_7101d47310, CRntString("MAINMENU"));
        self->to_main_menu = false;
    }
}
