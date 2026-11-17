// CSensorDoorComponent's unit (tools/tu_map.py): registers the door's two states (+0x108, +0x178) with its
// state machine (+0x58), binds each state's three callbacks to the component and asks for state 2.
// Names and layouts are placeholders.
#include "UnkDoorStateMachine_b58.h"
#include "types.h"

char* fn_71007b9524(char* p0);
char* fn_71007b9528(char* p0, f64 f0);
void fn_71007b952c();
char* fn_71007b9530(char* p0);
char* fn_71007b9534(char* p0, f64 f0);
void fn_71007b9538();

struct UnkSensorDoorState_l7b8118 : UnkDoorState_b58 {
    u8 pad_0x14[0x30 - 0x14];
    void* enter_owner;
    char* (*enter)(char*);
    void* leave_owner;
    void (*leave)();
    void* update_owner;
    char* (*update)(char*, f64);
    u8 pad_0x60[0x70 - 0x60];
};
static_assert(sizeof(UnkSensorDoorState_l7b8118) == 0x70, "layout");

struct UnkSensorDoor_l7b8118 {
    u8 pad_0x0[0x58];
    UnkDoorStateMachine_b58 state_machine;
    UnkSensorDoorState_l7b8118 closed;
    UnkSensorDoorState_l7b8118 opened;
};
static_assert(__builtin_offsetof(UnkSensorDoor_l7b8118, closed) == 0x108, "layout");
static_assert(__builtin_offsetof(UnkSensorDoor_l7b8118, opened) == 0x178, "layout");

static inline void AddState_l7b8118(UnkDoorStateMachine_b58* machine, UnkDoorState_b58* state) {
    u32 count = machine->count;
    UnkDoorStatePtr_b58* states = machine->states;
    for (u64 n = (u64)count * sizeof(UnkDoorStatePtr_b58), i = 0; i != n; i += sizeof(UnkDoorStatePtr_b58)) {
        if ((*states++)->id == state->id)
            return;
    }
    state->field_0x8 = reinterpret_cast<u64>(machine);
    machine->states[count] = state;
    machine->count = count + 1;
}

void fn_71007b8118(UnkSensorDoor_l7b8118* self) {
    self->state_machine.field_0x8 = reinterpret_cast<u64>(self);

    AddState_l7b8118(&self->state_machine, &self->closed);
    self->closed.enter_owner = self;
    self->closed.enter = fn_71007b9524;
    self->closed.update_owner = self;
    self->closed.update = fn_71007b9528;
    self->closed.leave_owner = self;
    self->closed.leave = fn_71007b952c;

    AddState_l7b8118(&self->state_machine, &self->opened);
    self->opened.enter_owner = self;
    self->opened.enter = fn_71007b9530;
    self->opened.update_owner = self;
    self->opened.update = fn_71007b9534;
    self->opened.leave_owner = self;
    self->opened.leave = fn_71007b9538;

    RequestState_b58(&self->state_machine, 2);
}
