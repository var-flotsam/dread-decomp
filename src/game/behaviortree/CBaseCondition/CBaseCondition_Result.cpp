// behaviortree::CBaseCondition (unit 499): the node's status from its test (virtual slot 20) and the
// "negate" flag at 0x2c: the status at 0x30 when the test passes after negation, else the one at
// 0x34. Names are placeholders.
#include "types.h"

#define UNK_VSLOT(n) virtual void vf_##n();

struct UnkBaseCondition {
    UNK_VSLOT(0) UNK_VSLOT(1) UNK_VSLOT(2) UNK_VSLOT(3) UNK_VSLOT(4) UNK_VSLOT(5) UNK_VSLOT(6)
    UNK_VSLOT(7) UNK_VSLOT(8) UNK_VSLOT(9) UNK_VSLOT(10) UNK_VSLOT(11) UNK_VSLOT(12) UNK_VSLOT(13)
    UNK_VSLOT(14) UNK_VSLOT(15) UNK_VSLOT(16) UNK_VSLOT(17) UNK_VSLOT(18) UNK_VSLOT(19)
    virtual bool vf_20();  // the test
    u8 pad_0x8[0x24];
    bool field_0x2c;  // negate
    s32 field_0x30;   // the status when it holds
    s32 field_0x34;   // the status when it does not
};

s32 fn_7100394538(UnkBaseCondition* self) {
    bool passed = self->vf_20();
    return passed == !self->field_0x2c ? self->field_0x30 : self->field_0x34;
}
