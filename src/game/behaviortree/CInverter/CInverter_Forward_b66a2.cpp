// behaviortree::CInverter unit (tools/tu_map.py, the name from reflection): passes the child status on to
// the object at +0x10, inverting 5 into 6 (every other status as 5). Names and layouts are placeholders.
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkInverterTarget_b66a2;

struct UnkInverter_b66a2 {
    u8 pad_0x0[0x10];
    UnkInverterTarget_b66a2* target;
};

void fn_71003a52d8(UnkInverter_b66a2* self, void* unused, s32 status) {
    fn_71003aac0c(self->target, self, status == 5 ? 6 : 5);
}
