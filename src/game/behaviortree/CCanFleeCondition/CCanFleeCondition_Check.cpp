// behaviortree::CCanFleeCondition's unit (tools/tu_map.py unit 523): caches the owner's (+0x18)
// +0x240 object at +0x38, then holds when that object is set, its flag +0x3e is on and its flag
// +0x14674 is off (1, else 0). Names are placeholders.
#include "types.h"

struct UnkInner523 {
    u8 pad_0x0[0x3e];
    bool field_0x3e;
    u8 pad_0x3f[0x14674 - 0x3f];
    bool field_0x14674;
};

struct UnkOwner523 {
    u8 pad_0x0[0x240];
    UnkInner523* inner;  // 0x240
};

struct UnkCondition523 {
    u8 pad_0x0[0x18];
    UnkOwner523* owner;  // 0x18
    u8 pad_0x20[0x18];
    UnkInner523* inner;  // 0x38
};

u32 fn_710039f628(UnkCondition523* self) {
    if (self->owner == nullptr) {
        return 0;
    }
    UnkInner523* inner = self->owner->inner;
    self->inner = inner;
    if (inner == nullptr || !inner->field_0x3e) {
        return 0;
    }
    if (inner->field_0x14674) {
        return 0;
    }
    u32 yes = 1;
    asm("" : "+r"(yes));
    return yes;
}
