// CEnemyMovement unit: takes a reference on the request's object, reads its name (or the empty
// string) as a state id and asks the state machine at +0x700 for it, when it is one of the seven.
// Names and layouts are placeholders.
#include "../CSensorDoorComponent/UnkDoorStateMachine_b58.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

u32 fn_71010cc114(const char* name);

struct UnkStateRequestObject_l0cd348 {
    u8 pad_0x0[0x10];
    const char* name;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 ref_count;  // 0x44
    };
};

struct UnkStateRequest_l0cd348 {
    u8 pad_0x0[0x10];
    UnkStateRequestObject_l0cd348* object;
};

struct UnkEnemyMovement_l0cd348 {
    u8 pad_0x0[0x700];
    UnkDoorStateMachine_b58 state_machine;
};
static_assert(__builtin_offsetof(UnkEnemyMovement_l0cd348, state_machine.pending) == 0x7a4, "layout");

void fn_71010cd348(UnkEnemyMovement_l0cd348* self, UnkStateRequest_l0cd348* request) {
    ++request->object->ref_count;
    const char* name = request->object->name;
    u32 state = fn_71010cc114(name ? name : g_unk_710162e2ed);
    UnkDoorStateMachine_b58* machine = &self->state_machine;
    machine->pending = state;
    if ((state <= 6) & !machine->field_0xa8) {
        UnkDoorState_b58* current = machine->current;
        if (!current || current->id != static_cast<s32>(state)) {
            if (UnkDoorState_b58* next = FindState_b58(machine, state)) {
                if (current) {
                    current->vf_0x18(state);
                    machine->vf_0x28(machine->current, next);
                }
                machine->previous = machine->current;
                machine->current = next;
                next->vf_0x10(machine->previous ? machine->previous->id : 0x7fffffff);
                machine->vf_0x20(machine->previous, next);
            }
        }
        machine->pending = 0x7fffffff;
    }
}
