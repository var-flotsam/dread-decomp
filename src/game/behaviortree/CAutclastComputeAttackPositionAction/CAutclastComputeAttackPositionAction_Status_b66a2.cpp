// behaviortree::CAutclastComputeAttackPositionAction unit (tools/tu_map.py, the name from reflection): the
// status it reports, 6 while its flag at +0x38 is clear, else 5. Names and layouts are placeholders.
#include "types.h"

struct UnkAutclastComputeAttackPositionAction_b66a2 {
    u8 pad_0x0[0x38];
    bool done;
};

s32 fn_71003b3800(UnkAutclastComputeAttackPositionAction_b66a2* self) {
    return self->done ? 5 : 6;
}
