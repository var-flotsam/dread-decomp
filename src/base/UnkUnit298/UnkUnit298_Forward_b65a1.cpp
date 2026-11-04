// Unit 298 of tools/tu_map.py (no reflected class): forwarders that pass their arguments on to a virtual
// of an object held by the receiver. Names and layouts are placeholders.
#include "types.h"

struct UnkForwardTarget298_b65a1 {
    void** vtable;
};

struct UnkForwardOwner298_b65a1 {
    u8 pad_0x0[0x10];
    UnkForwardTarget298_b65a1* target;
};

void fn_71002100f4(UnkForwardOwner298_b65a1* self, u64 a, u64 b) {
    typedef void (*Fn)(UnkForwardTarget298_b65a1*, u64, u64);
    UnkForwardTarget298_b65a1* target = self->target;
    reinterpret_cast<Fn>(target->vtable[0x28 / 8])(target, a, b);
}
