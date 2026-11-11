// CQuarentineDoorComponent's unit (tools/tu_map.py): fn_71007df6cc sets the flag at +0x2a0, asks the
// door's state machine (+0xa0) for state 3, sends the owner's (+0x30) sub-object at +0x220, if any,
// "opened" with mode 2 (../CSensorDoorComponent/UnkDoorStateMachine_b58.h), then fn_71007df12c(1).
// Names are placeholders.
#include "../CSensorDoorComponent/UnkDoorStateMachine_b58.h"
#include "game/CQuarentineDoorComponent.h"

struct UnkQuarentineDoor_b54;

struct UnkQuarentineOwner_b58 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkQuarentineDoor_b58 {
    u8 pad_0x0[0x30];
    UnkQuarentineOwner_b58* owner;
    u8 pad_0x38[0xa0 - 0x38];
    UnkDoorStateMachine_b58 state_machine;
    u8 pad_0x150[0x2a0 - 0x150];
    bool field_0x2a0;
};

void fn_71007df6cc(UnkQuarentineDoor_b58* self) {
    self->field_0x2a0 = true;
    RequestState_b58(&self->state_machine, 3);
    if (self->owner)
        if (void* target = self->owner->field_0x220)
            SendOpened_b58(target);
    fn_71007df12c(reinterpret_cast<UnkQuarentineDoor_b54*>(self), true);
}
