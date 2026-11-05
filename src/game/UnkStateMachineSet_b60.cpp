// A component's small state machine asked for a new state (the machine of
// src/game/UnkStateMachineUpdate_b59.cpp, embedded in its component at some offset, followed by a
// "busy" flag at +0xa8): the request stores the pending id, gives up past the last state or while busy,
// and otherwise switches state as the b59 "apply pending" does. One template per (last state, "none"
// value), instantiated per component and machine offset; a constant id folds away the range test and
// the id-0 path. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkSmState_b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b60* __attribute__((aligned(4))) UnkSmStatePtr_b60;

struct UnkSmMachine_b60 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b60* previous, UnkSmState_b60* next);
    virtual void vf_0x28(UnkSmState_b60* current, UnkSmState_b60* next);

    u64 field_0x8;
    UnkSmState_b60* current;
    UnkSmState_b60* previous;
    u32 count;
    UnkSmStatePtr_b60 states[16];
    u32 pending;
    bool busy;
};

template <u32 Offset>
struct UnkSmOwner_b60 {
    u8 pad_0x0[Offset];
    UnkSmMachine_b60 machine;
};

__attribute__((always_inline)) static inline bool FindState_b60(UnkSmMachine_b60* self, u32 id,
                                                               UnkSmState_b60*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b60);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b60* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b60)) {
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
__attribute__((always_inline)) static inline void SwitchState_b60(UnkSmMachine_b60* self, u32 id,
                                                                 UnkSmState_b60* current) {
    if (!current || current->id != id) {
        UnkSmState_b60* found;
        UnkSmState_b60* next;
        u32 enter;
        if (FindState_b60(self, id, found)) {
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
            typedef void (*EnteredFn)(UnkSmState_b60*, s32);
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
__attribute__((always_inline)) static inline void RequestState_b60(UnkSmMachine_b60* self, u32 id) {
    // Stored through a plain u32 pointer: the original rereads a state id the caller tested after this.
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_b60<None>(self, id, self->current);
}

#define UNK_SM_SET_B60(addr, offset, last, none)                   \
    void fn_##addr(UnkSmOwner_b60<offset>* self, u32 id) {           \
        RequestState_b60<last, none>(&self->machine, id);            \
    }

#define UNK_SM_SET_CONST_B60(addr, offset, id, last, none)          \
    void fn_##addr(UnkSmOwner_b60<offset>* self) {                   \
        RequestState_b60<last, none>(&self->machine, id);            \
    }

UNK_SM_SET_B60(71007f9428, 0x228, 4, 5)
UNK_SM_SET_CONST_B60(71007b8014, 0x58, 2, 2, 0x7fffffff)
UNK_SM_SET_CONST_B60(7100818ab8, 0x480, 1, 4, 5)
UNK_SM_SET_CONST_B60(7100918a78, 0x40, 3, 3, 0x7fffffff)

// The same request after a base call (each one's own unit; signatures copied where declared).
struct UnkParent;
struct UnkEvacuationCountDown;
struct UnkMorphBallLauncher_b57;
struct UnkLinkList;
struct UnkSmBase_b60;
void fn_71010c4228(UnkParent*);
void fn_71007dbeac(UnkSmBase_b60*);
void fn_710024facc(UnkSmBase_b60*);
void fn_7100818bc0(UnkSmBase_b60*);
void fn_71007dc10c(UnkEvacuationCountDown*);
void fn_71010c422c(UnkMorphBallLauncher_b57*);
void fn_71002aeb9c(UnkLinkList*, UnkLinkList**);
void fn_71008656d8(void*, u32, u32, void*);
bool fn_71007b3318(UnkSmBase_b60*);

#define UNK_SM_SET_AFTER_B60(addr, base, basetype, offset, id, last, none) \
    void fn_##addr(UnkSmOwner_b60<offset>* self) {                           \
        base(reinterpret_cast<basetype*>(self));                             \
        RequestState_b60<last, none>(&self->machine, id);                    \
    }

UNK_SM_SET_AFTER_B60(71007b7df0, fn_71010c4228, UnkParent, 0x58, 2, 2, 0x7fffffff)
UNK_SM_SET_AFTER_B60(71007de4f0, fn_71007dbeac, UnkSmBase_b60, 0xa0, 2, 2, 0x7fffffff)
UNK_SM_SET_AFTER_B60(71007f931c, fn_710024facc, UnkSmBase_b60, 0x228, 1, 4, 5)
UNK_SM_SET_AFTER_B60(710081c584, fn_7100818bc0, UnkSmBase_b60, 0x480, 3, 4, 5)
UNK_SM_SET_AFTER_B60(710091eb20, fn_710024facc, UnkSmBase_b60, 0x280, 1, 5, 6)

struct UnkEvacuationSm_b60 {
    u8 pad_0x0[0x30];
    void* field_0x30;
    u8 pad_0x38[0x7a - 0x38];
    bool field_0x7a;
    u8 pad_0x7b[0xa0 - 0x7b];
    UnkSmMachine_b60 machine;
};

void fn_71007de6e4(UnkEvacuationSm_b60* self) {
    fn_71007dc10c(reinterpret_cast<UnkEvacuationCountDown*>(self));
    if (self->field_0x7a && self->field_0x30)
        RequestState_b60<2, 0x7fffffff>(&self->machine, 1);
}

struct UnkFrozenSm_b60 {
    u8 pad_0x0[0x228];
    UnkSmMachine_b60 machine;
    u8 pad_0x2d8[0x360 - 0x2d8];
    UnkLinkList* field_0x360;
};

void fn_71007f97ec(UnkFrozenSm_b60* self) {
    fn_71010c422c(reinterpret_cast<UnkMorphBallLauncher_b57*>(self));
    if (UnkLinkList* list = self->field_0x360) {
        fn_71002aeb9c(list, &self->field_0x360);
        self->field_0x360 = nullptr;
    }
    RequestState_b60<4, 5>(&self->machine, 1);
}

struct UnkLauncherOwner_b60 {
    u8 pad_0x0[0x2a0];
    void* field_0x2a0;
};

struct UnkLauncherSm_b60 {
    u8 pad_0x0[0x30];
    UnkLauncherOwner_b60* owner;
    u8 pad_0x38[0x40 - 0x38];
    u8 field_0x40[0x10];
    u8 pad_0x50[0x208 - 0x50];
    UnkSmMachine_b60 machine;
};

void fn_71008868e0(UnkLauncherSm_b60* self) {
    fn_71008656d8(self->owner->field_0x2a0, 0x10000, 0x10, self->field_0x40);
    RequestState_b60<4, 5>(&self->machine, 1);
}

struct UnkTimerSm_b60 {
    u8 pad_0x0[0x260];
    UnkSmMachine_b60 machine;
    u8 pad_0x310[0x3f0 - 0x310];
    f32 timer;
    u8 pad_0x3f4[0x468 - 0x3f4];
    f32 field_0x468;
};

void fn_710088ba98(UnkTimerSm_b60* self, f32 dt) {
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (!(self->timer <= 0.0f))
            return;
        RequestState_b60<3, 0x7fffffff>(&self->machine, 3);
    }
}

struct UnkTimerSm2_b60 {
    u8 pad_0x0[0x280];
    UnkSmMachine_b60 machine;
    u8 pad_0x330[0x344 - 0x330];
    f32 timer;
};

void fn_710091ff88(UnkTimerSm2_b60* self, f32 dt) {
    self->timer -= dt;
    if (!(self->timer <= 0.0f))
        return;
    self->timer = 0.0f;
    RequestState_b60<5, 6>(&self->machine, 3);
}

struct UnkGuardedSm_b60 {
    u8 pad_0x0[0x40];
    UnkSmMachine_b60 machine;
    u8 pad_0xf0[0x290 - 0xf0];
    bool field_0x290;
};

void fn_71007b3204(UnkGuardedSm_b60* self) {
    if (self->field_0x290)
        return;
    if (fn_71007b3318(reinterpret_cast<UnkSmBase_b60*>(self)))
        return;
    RequestState_b60<3, 4>(&self->machine, 2);
}

struct UnkSmOther_b60 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};
UnkSmOther_b60* fn_710097f198(UnkSmBase_b60*);

void fn_7100980cd0(UnkSmOwner_b60<0x30>* self) {
    if (fn_710097f198(reinterpret_cast<UnkSmBase_b60*>(self))->field_0x3a8)
        RequestState_b60<3, 4>(&self->machine, 3);
}

// Requests that depend on the current state.
static inline u32 StateId_b60(UnkSmState_b60* state) {
    return *reinterpret_cast<u32*>(reinterpret_cast<char*>(state) + 0x10);
}

void fn_71008189a4(UnkSmOwner_b60<0x480>* self) {
    UnkSmState_b60* current = self->machine.current;
    if (current && (StateId_b60(current) == 4 || StateId_b60(current) == 1))
        RequestState_b60<4, 5>(&self->machine, 2);
}

struct UnkGrappleSm_b60 {
    u8 pad_0x0[0x480];
    UnkSmMachine_b60 machine;
    u8 pad_0x530[0x714 - 0x530];
    bool field_0x714;
    bool field_0x715;
};

void fn_710081b510(UnkGrappleSm_b60* self) {
    self->field_0x715 = false;
    UnkSmState_b60* current = self->machine.current;
    if (current && StateId_b60(current) == 3)
        RequestState_b60<4, 5>(&self->machine, 4);
}

// Units that request a state and then apply whatever is pending: when the machine is busy the request
// only records the id and the apply switches; otherwise the request switches and the apply finds
// nothing pending (two inlined copies of the switch in the original).
template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void ApplyPending_b60(UnkSmMachine_b60* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    SwitchState_b60<None>(self, id, self->current);
}

void fn_710097cba8(UnkSmOwner_b60<0x30>* self) {
    RequestState_b60<4, 5>(&self->machine, 1);
    ApplyPending_b60<4, 5>(&self->machine);
}

void fn_710098306c(UnkSmOwner_b60<0x30>* self) {
    RequestState_b60<3, 4>(&self->machine, 2);
    ApplyPending_b60<3, 4>(&self->machine);
}

void fn_71009832e4(UnkSmOwner_b60<0x30>* self) {
    RequestState_b60<3, 4>(&self->machine, 1);
    ApplyPending_b60<3, 4>(&self->machine);
}

// Requests behind a guard.
struct UnkFrozenGuarded_b60 {
    u8 pad_0x0[0x228];
    UnkSmMachine_b60 machine;
    u8 pad_0x2d8[0x398 - 0x2d8];
    bool field_0x398;
};

void fn_71007fabe4(UnkFrozenGuarded_b60* self) {
    if (self->field_0x398)
        return;
    RequestState_b60<4, 5>(&self->machine, 1);
}

struct UnkPauseState_b60 {
    u8 pad_0x0[0x21];
    bool field_0x21;
};
extern UnkPauseState_b60 g_unk_71727210c8;

struct UnkFrozenTimer_b60 {
    u8 pad_0x0[0x40];
    f32 timer;
    u8 pad_0x44[0x228 - 0x44];
    UnkSmMachine_b60 machine;
};

void fn_71007fa48c(UnkFrozenTimer_b60* self, f32 dt) {
    if (g_unk_71727210c8.field_0x21)
        return;
    self->timer -= dt;
    if (!(self->timer <= 0.0f))
        return;
    RequestState_b60<4, 5>(&self->machine, 3);
}

struct UnkSmLink2_b60 {
    u8 pad_0x0[0x2a8];
    void* field_0x2a8;
};
struct UnkSmLink1_b60 {
    u8 pad_0x0[0x10];
    UnkSmLink2_b60* field_0x10;
};
void fn_710083b0b4(void*, void*);
extern const u64 g_unk_710164aea0 __attribute__((visibility("hidden")));

struct UnkLauncherSm2_b60 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x208 - 0x38];
    UnkSmMachine_b60 machine;
    u8 pad_0x2b8[0x300 - 0x2b8];
    bool field_0x300;
    u8 pad_0x301[0x310 - 0x301];
    UnkSmLink1_b60* field_0x310;
    u8 pad_0x318[0x368 - 0x318];
    u8 field_0x368[8];
};

void fn_7100887310(UnkLauncherSm2_b60* self) {
    if (self->field_0x300) {
        fn_710083b0b4(self->field_0x310->field_0x10->field_0x2a8, self->field_0x368);
        fn_7100d7c828(self->actor, &g_unk_710164aea0, false);
        return;
    }
    RequestState_b60<4, 5>(&self->machine, 3);
}

struct UnkSmTunable_b60 {
    u8 pad_0x0[0x2c];
    f32 field_0x2c;
};
extern base::reflection::UnkTunable g_unk_71726f3440;
struct UnkSmTarget_b60 {
    u8 pad_0x0[0x30];
    void* field_0x30;
};
UnkSmTarget_b60* fn_71005b6320(void*);

struct UnkSmTimerUp_b60 {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x40 - 0x38];
    UnkSmMachine_b60 machine;
    u8 pad_0xf0[0x2b0 - 0xf0];
    f32 timer;
};

void fn_7100918dbc(UnkSmTimerUp_b60* self, f32 dt) {
    self->timer += dt;
    if (self->timer >= reinterpret_cast<const UnkSmTunable_b60*>(&g_unk_71726f3440)->field_0x2c) {
    } else {
        UnkSmTarget_b60* target = fn_71005b6320(self->owner);
        if (!target || !target->field_0x30)
            return;
    }
    RequestState_b60<4, 0x7fffffff>(&self->machine, 4);
}

bool fn_7100819d6c(UnkGrappleSm_b60* self) {
    if (!self->field_0x714)
        return false;
    self->field_0x714 = false;
    UnkSmState_b60* current = self->machine.current;
    if (current && StateId_b60(current) == 2)
        RequestState_b60<4, 5>(&self->machine, 4);
    return true;
}

struct UnkSlidingSm_b60 {
    u8 pad_0x0[0x48];
    bool field_0x48;
    u8 pad_0x49[0x280 - 0x49];
    UnkSmMachine_b60 machine;
};

void fn_710091f22c(UnkSlidingSm_b60* self) {
    self->field_0x48 = true;
    UnkSmState_b60* current = self->machine.current;
    if (!current)
        return;
    if (StateId_b60(current) == 1)
        RequestState_b60<5, 6>(&self->machine, 2);
    else if (StateId_b60(current) == 5)
        RequestState_b60<5, 6>(&self->machine, 3);
}

bool fn_710088b7dc(UnkTimerSm_b60*);

void fn_710088bd1c(UnkTimerSm_b60* self, f32 dt) {
    if (!(self->field_0x468 >= 0.0f))
        return;
    self->field_0x468 -= dt;
    if (!(self->field_0x468 <= 0.0f))
        return;
    if (!fn_710088b7dc(self))
        RequestState_b60<3, 0x7fffffff>(&self->machine, 1);
    self->field_0x468 = 0.0f;
}

struct UnkComponent;
struct UnkCameraCtrl;
void* fn_710083cd30(UnkComponent*);
bool fn_7100023d00(const UnkCameraCtrl*, const base::reflection::CClass*);

bool fn_7100888c24(UnkLauncherSm2_b60* self, s32 event) {
    if (event == 1) {
        const UnkCameraCtrl* ctrl = static_cast<const UnkCameraCtrl*>(
            fn_710083cd30(static_cast<UnkComponent*>(self->field_0x310->field_0x10->field_0x2a8)));
        if (fn_7100023d00(ctrl, fn_710082ff24()) || fn_7100023d00(ctrl, fn_710084a264()))
            RequestState_b60<4, 5>(&self->machine, 3);
    }
    return true;
}

bool fn_7100889040(UnkSmOwner_b60<0x208>* self, const s32* message) {
    if (*message == 1 || *message == 2) {
        RequestState_b60<4, 5>(&self->machine, 1);
        return false;
    }
    return true;
}
