// Unit 399 of tools/tu_map.py (no reflected class): forwards its arguments to a virtual (slot 0x78) of the
// object the receiver's +0x38 field points at. Names and layouts are placeholders.
#include "types.h"

struct UnkForwardTarget399_b66a2 {
    void** vtable;
};

struct UnkForwardOwner399_b66a2 {
    u8 pad_0x0[0x38];
    UnkForwardTarget399_b66a2** target;
};

void fn_71002d5958(UnkForwardOwner399_b66a2* self, u64 a, u64 b) {
    typedef void (*Fn)(UnkForwardTarget399_b66a2*, u64, u64);
    UnkForwardTarget399_b66a2* target = *self->target;
    reinterpret_cast<Fn>(target->vtable[0x78 / 8])(target, a, b);
}
