// A component's small state machine applying its pending state (the same object as
// src/game/CSensorDoorComponent/UnkDoorStateMachine_b58.h: current +0x10, previous +0x18, count +0x20,
// states +0x24, pending +0xa4). One template per (last state, "none" value), instantiated once per
// component unit: CCentralUnitComponent, CCharClassEmmyValveComponent, CSensorDoorComponent,
// CQuarentineDoorComponent, CFrozenComponent, CGrappleBeamComponent, CMorphBallLauncherComponent,
// CWaterNozzleComponent, CDoorShieldLifeComponent, CMagmaKraidPistonPlatformComponent,
// CMagnetSlidingBlockComponent and three unnamed units. Pending id 0 with no state leaves the machine
// with no current state. The find reports success as a bool (a null entry counts as not found), and the
// "enter" flag and the cleared `next` are kept in registers of their own (the barriers), as the original
// does. Names are placeholders.
#include "types.h"

struct UnkSmState_b59 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    u32 id;
};

typedef UnkSmState_b59* __attribute__((aligned(4))) UnkSmStatePtr_b59;

struct UnkSmMachine_b59 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkSmState_b59* previous, UnkSmState_b59* next);
    virtual void vf_0x28(UnkSmState_b59* current, UnkSmState_b59* next);

    u64 field_0x8;
    UnkSmState_b59* current;
    UnkSmState_b59* previous;
    u32 count;
    UnkSmStatePtr_b59 states[16];
    u32 pending;
};

__attribute__((always_inline)) static inline bool FindState_b59(UnkSmMachine_b59* self, u32 id,
                                                               UnkSmState_b59*& out) {
    u64 n = (u64)self->count * sizeof(UnkSmStatePtr_b59);
    asm("" : "+r"(n));
    for (UnkSmStatePtr_b59* p = self->states; n != 0; ++p, n -= sizeof(UnkSmStatePtr_b59)) {
        if ((*p)->id == id) {
            out = *p;
            return out != nullptr;
        }
    }
    out = nullptr;
    return false;
}

template <u32 Last, u32 None>
__attribute__((always_inline)) static inline void ApplyPending_b59(UnkSmMachine_b59* self) {
    u32 id = self->pending;
    if (id > Last)
        return;
    UnkSmState_b59* current = self->current;
    if (!current || current->id != id) {
        UnkSmState_b59* found;
        UnkSmState_b59* next;
        u32 enter;
        if (FindState_b59(self, id, found)) {
            next = found;
            enter = true;
        } else {
            if (id != 0)
                goto done;
            enter = false;
            next = nullptr;
        }
        if (current) {
            current->vf_0x18(id);
            self->vf_0x28(self->current, next);
        }
        self->previous = self->current;
        self->current = found;
        if (enter) {
            // The original reads the entered slot from the stored pointer and calls it on `next`
            // (the same object on this path).
            typedef void (*EnteredFn)(UnkSmState_b59*, s32);
            s32 previous_id = self->previous ? self->previous->id : None;
            EnteredFn entered = (*reinterpret_cast<EnteredFn**>(found))[2];
            entered(next, previous_id);
            self->vf_0x20(self->previous, next);
        }
    }
done:
    self->pending = None;
}

#define UNK_SM_APPLY_B59(addr, last, none) \
    void fn_##addr(UnkSmMachine_b59* self) { ApplyPending_b59<last, none>(self); }

UNK_SM_APPLY_B59(710077bbd0, 7, 8)
UNK_SM_APPLY_B59(71007b4c78, 3, 4)
UNK_SM_APPLY_B59(71007b941c, 2, 0x7fffffff)
UNK_SM_APPLY_B59(71007dfcf4, 3, 0x7fffffff)
UNK_SM_APPLY_B59(71007fb6ac, 4, 5)
UNK_SM_APPLY_B59(7100821be0, 4, 5)
UNK_SM_APPLY_B59(71008887d0, 4, 5)
UNK_SM_APPLY_B59(71008a26a0, 4, 5)
UNK_SM_APPLY_B59(71008dd0c8, 3, 0x7fffffff)
UNK_SM_APPLY_B59(71009198fc, 4, 0x7fffffff)
UNK_SM_APPLY_B59(7100921ba8, 5, 6)
UNK_SM_APPLY_B59(710097caa0, 4, 5)
UNK_SM_APPLY_B59(71009805b4, 3, 4)
UNK_SM_APPLY_B59(71009831dc, 3, 4)
