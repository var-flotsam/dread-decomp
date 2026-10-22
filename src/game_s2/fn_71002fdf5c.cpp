// Calls virtual slot 0xf8 on each child object of the array at +0x40, passing `arg` through.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_71002fdf5c(UnkParent* self, void* arg) {
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*, void*)>((*p)->vtable[0xf8 / 8])(*p, arg);
}
