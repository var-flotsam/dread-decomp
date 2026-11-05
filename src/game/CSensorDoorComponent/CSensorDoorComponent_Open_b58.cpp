// CSensorDoorComponent's unit (tools/tu_map.py): fn_71007b8e48 sets the flag at +0x1e8, asks the door's
// state machine (+0x58) for state 1, sends the owner's (+0x30) sub-object at +0x220, if any, "opened"
// with mode 2 (UnkDoorStateMachine_b58.h) and gives the owner's weak target (fn_71002b3b84) the hash
// fn_71007b877c gives it (fn_71009c0f1c). Names are placeholders.
#include "UnkDoorStateMachine_b58.h"

struct UnkWeakTarget;
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void* object, u64 id);

struct UnkSensorDoorOwner_b58 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSensorDoor_b58 {
    u8 pad_0x0[0x30];
    UnkSensorDoorOwner_b58* owner;
    u8 pad_0x38[0x58 - 0x38];
    UnkDoorStateMachine_b58 state_machine;
    u8 pad_0x108[0x1e8 - 0x108];
    bool field_0x1e8;
};

void fn_71007b8e48(UnkSensorDoor_b58* self) {
    self->field_0x1e8 = true;
    RequestState_b58(&self->state_machine, 1);
    if (self->owner)
        if (void* target = self->owner->field_0x220)
            SendOpened_b58(target);
    if (void* weak = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(weak, 0x6909d27d2c787fba);
}
