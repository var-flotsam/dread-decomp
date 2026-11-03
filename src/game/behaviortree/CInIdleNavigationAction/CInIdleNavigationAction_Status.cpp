// behaviortree::CInIdleNavigationAction's unit (tools/tu_map.py unit 487): the action's status from
// two flags: 6 when either +0x38 or +0x39 is set, 5 otherwise. Names are placeholders.
#include "types.h"

struct UnkInIdleNavigationAction {
    u8 pad_0x0[0x38];
    bool field_0x38;
    bool field_0x39;
};

s32 fn_710038a678(UnkInIdleNavigationAction* self) {
    if (self->field_0x38) {
        return 6;
    }
    return self->field_0x39 ? 6 : 5;
}
