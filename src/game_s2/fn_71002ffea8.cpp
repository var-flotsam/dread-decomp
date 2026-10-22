// Runs fn_71010c424c, then calls virtual slot 0x50 on each child object of the array at +0x40, passing `dt` through.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_71010c424c(UnkParent*);

void fn_71002ffea8(UnkParent* self, float dt) {
    fn_71010c424c(self);
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*, float)>((*p)->vtable[0x50 / 8])(*p, dt);
}
