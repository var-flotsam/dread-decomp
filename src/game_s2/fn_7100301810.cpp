// Calls virtual slot 0x98 on each child object of the array at +0x40, passing the float `dt` through.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_7100301810(UnkParent* self, float dt) {
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*, float)>((*p)->vtable[0x98 / 8])(*p, dt);
}
