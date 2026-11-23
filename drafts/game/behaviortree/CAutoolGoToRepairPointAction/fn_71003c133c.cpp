// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): advances the action's state machine
// (+0x2d0, "none" 7): from state 1 it sends the owner's (+0x18) sub-object at +0x220 the hash at +0x3a0
// when one is set (UnkSpitSend) and moves to state 2; from state 4 it moves to state 5. Names and layouts
// are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "../../../../src/game/UnkSmRequest_A5_b73.h"
#include "types.h"

struct UnkAutoolOwner_l3c133c {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkAutoolGoToRepairPoint_l3c133c {
    u8 pad_0x0[0x18];
    UnkAutoolOwner_l3c133c* owner;
    u8 pad_0x20[0x2d0 - 0x20];
    UnkSmMachine_A5_b73 machine;  // 0x2d0
    u8 pad_0x380[0x3a0 - 0x380];
    u64 field_0x3a0;
};
static_assert(__builtin_offsetof(UnkAutoolGoToRepairPoint_l3c133c, field_0x3a0) == 0x3a0, "layout");

void fn_71003c133c(UnkAutoolGoToRepairPoint_l3c133c* self) {
    UnkSmState_A5_b73* current = self->machine.current;
    if (!current)
        return;
    switch (current->id) {
    case 1:
        if (self->field_0x3a0 != (u64)-1)
            UnkSpitSend(self->owner->field_0x220, self->field_0x3a0, 0xf4061ff25a2163d6);
        RequestState_A5_b73<15, 7>(&self->machine, 2);
        break;
    case 4:
        RequestState_A5_b73<15, 7>(&self->machine, 5);
        break;
    }
}
