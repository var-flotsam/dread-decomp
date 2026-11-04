// behaviortree::CComputeTargetPositionAction unit (tools/tu_map.py, the name from reflection): the status
// it reports, 6 while its flag at +0x2c is clear, else 5. Names and layouts are placeholders.
#include "types.h"

struct UnkComputeTargetPositionAction_b66a2 {
    u8 pad_0x0[0x2c];
    bool done;
};

s32 fn_710037dfe4(UnkComputeTargetPositionAction_b66a2* self) {
    return self->done ? 5 : 6;
}
