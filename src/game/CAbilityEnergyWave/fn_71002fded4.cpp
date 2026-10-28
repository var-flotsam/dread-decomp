// Calls virtual slot 0xf0 on each child object of the array at +0x40, passing six arguments through.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_71002fded4(UnkParent* self, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6) {
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*, void*, void*, void*, void*, void*, void*)>((*p)->vtable[0xf0 / 8])(*p, a1, a2, a3, a4, a5, a6);
}
