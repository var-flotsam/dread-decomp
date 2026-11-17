// CCentralUnitCannonAIComponent unit (tools/tu_map.py): the start: after the base's part (fn_71010fa290),
// keeps the owner's heading (atan2 of two entries of its matrix) three times from +0x146b8, asks the state
// machine at +0x14978 for state 1 and clears the flag at +0x146d0. Names and layouts are placeholders.
#include <math.h>
#include "base/UnkMatrix44.h"
#include "game/UnkStateMachineFind.h"
#include "types.h"

void fn_71010fa290(char* self);

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

static inline bool FindState_l3fbf60(UnkSmMachine_b61* self, u32 id, UnkSmState_b61*& out) {
    out = FindStateById(self->states, self->count, id);
    return out != nullptr;
}

// Switches to state `id` and clears the pending id.
template <u32 None>
static inline void SwitchState_l3fbf60(UnkSmMachine_b61* self, u32 id, UnkSmState_b61* current) {
    if (!current || current->id != id) {
        UnkSmState_b61* found;
        UnkSmState_b61* next;
        u32 enter;
        if (FindState_l3fbf60(self, id, found)) {
            next = found;
            enter = true;
        } else {
            // The original gives up here for any state but 0.
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

template <u32 Last, u32 None>
static inline void RequestState_l3fbf60(UnkSmMachine_b61* self, u32 id) {
    u32* pending = &self->pending;
    *pending = id;
    if (id > Last)
        return;
    if (self->busy)
        return;
    SwitchState_l3fbf60<None>(self, id, self->current);
}

struct UnkCannonOwner_l3fbf60 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkCannonAI_l3fbf60 {
    u8 pad_0x0[0x30];
    UnkCannonOwner_l3fbf60* owner;
    u8 pad_0x38[0x14680];
    f32 heading[3];  // 0x146b8
    u8 pad_0x146c4[0xc];
    bool field_0x146d0;
    u8 pad_0x146d1[0x2a7];
    UnkSmMachine_b61 machine;  // 0x14978
};
static_assert(__builtin_offsetof(UnkCannonAI_l3fbf60, field_0x146d0) == 0x146d0, "layout");
static_assert(__builtin_offsetof(UnkCannonAI_l3fbf60, machine) == 0x14978, "layout");

void fn_71003fbf60(UnkCannonAI_l3fbf60* self) {
    fn_71010fa290(reinterpret_cast<char*>(self));
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 x = m.m[1];
    f32 y = m.m[5];
    f32 heading = atan2f(y, x);
    self->heading[0] = heading;
    self->heading[1] = heading;
    self->heading[2] = heading;
    RequestState_l3fbf60<3, 0x7fffffff>(&self->machine, 1);
    self->field_0x146d0 = false;
}
