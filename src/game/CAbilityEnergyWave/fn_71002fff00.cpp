// Runs fn_710024fb28, then calls virtual slot 0x58 on each child object of the array at +0x40, passing `dt` through.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_710024fb28(UnkParent*);

void fn_71002fff00(UnkParent* self, float dt) {
    fn_710024fb28(self);
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*, float)>((*p)->vtable[0x58 / 8])(*p, dt);
}
