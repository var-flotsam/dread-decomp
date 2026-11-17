// CDoorLifeComponent's unit (tools/tu_map.py unit 1376): unless the door's state machine is already in
// state 1, requests state 1 from it, then starts a blend to 1.0 when enabled. Names and layouts are placeholders.
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU1376.h"
#include "types.h"

struct UnkDoorState_l8d0ef4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

// The states are only 4-aligned after the count.
typedef UnkDoorState_l8d0ef4* __attribute__((aligned(4))) UnkDoorStatePtr_l8d0ef4;

struct UnkDoorStateMachine_l8d0ef4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkDoorState_l8d0ef4* previous, UnkDoorState_l8d0ef4* next);  // after entering
    virtual void vf_0x28(UnkDoorState_l8d0ef4* current, UnkDoorState_l8d0ef4* next);   // after leaving

    u64 field_0x8;
    UnkDoorState_l8d0ef4* current;
    UnkDoorState_l8d0ef4* previous;
    u32 count;
    UnkDoorStatePtr_l8d0ef4 states[16];
    s32 pending;
    bool busy;
};

struct UnkDoorLife_l8d0ef4 {
    u8 pad_0x0[0x41c];
    bool field_0x41c;
    UnkDoorStateMachine_l8d0ef4 machine;
    u8 pad_0x4d0[0x638 - 0x4d0];
    f32 field_0x638;
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d0ef4, machine) == 0x420, "layout");
static_assert(__builtin_offsetof(UnkDoorStateMachine_l8d0ef4, pending) == 0xa4, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d0ef4, field_0x638) == 0x638, "layout");

static inline UnkDoorState_l8d0ef4* FindState(UnkDoorStateMachine_l8d0ef4* self, s32 id) {
    return FindStateById(self->states, self->count, id);
}

static inline void RequestState(UnkDoorStateMachine_l8d0ef4* self, s32 id) {
    self->pending = id;
    if (self->busy)
        return;
    UnkDoorState_l8d0ef4* current = self->current;
    if (!current || current->id != id) {
        if (UnkDoorState_l8d0ef4* next = FindState(self, id)) {
            if (current) {
                current->vf_0x18(id);
                self->vf_0x28(self->current, next);
            }
            self->previous = self->current;
            self->current = next;
            next->vf_0x10(self->previous ? self->previous->id : 0x7fffffff);
            self->vf_0x20(self->previous, next);
        }
    }
    self->pending = 0x7fffffff;
}

bool fn_71008d0ef4(void* p) {
    UnkDoorLife_l8d0ef4* self = static_cast<UnkDoorLife_l8d0ef4*>(p);
    UnkDoorState_l8d0ef4* current = self->machine.current;
    if (current && current->id == 1)
        return false;
    RequestState(&self->machine, 1);
    if (self->field_0x41c)
        fn_71008d0c24(reinterpret_cast<UnkDoorLifeA5_b61*>(self), 1.0f, self->field_0x638);
    return true;
}
