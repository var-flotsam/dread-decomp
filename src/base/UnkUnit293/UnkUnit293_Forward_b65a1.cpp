// Unit 293 of tools/tu_map.py (no reflected class): forwarders that pass their arguments on to a virtual
// of an object held by the receiver. Names and layouts are placeholders.
#include "types.h"

struct UnkForwardTarget293_b65a1 {
    void** vtable;
};

struct UnkForwardOwner293_b65a1 {
    u8 pad_0x0[0x20];
    UnkForwardTarget293_b65a1* target;
};

void fn_710020e4ec(UnkForwardOwner293_b65a1* self, u64 a, u64 b) {
    typedef void (*Fn)(UnkForwardTarget293_b65a1*, u64, u64);
    UnkForwardTarget293_b65a1* target = self->target;
    reinterpret_cast<Fn>(target->vtable[0x28 / 8])(target, a, b);
}

void fn_710020e868(UnkForwardOwner293_b65a1* self, u64 a, u64 b, u64 c) {
    typedef void (*Fn)(UnkForwardTarget293_b65a1*, u64, u64, u64);
    UnkForwardTarget293_b65a1* target = self->target;
    reinterpret_cast<Fn>(target->vtable[0x70 / 8])(target, a, b, c);
}
