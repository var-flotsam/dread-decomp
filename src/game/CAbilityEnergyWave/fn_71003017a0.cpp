// Calls virtual slot 0x128 on each child object of the array at +0x40, collecting a result through an out parameter.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

u32 fn_71003017a0(UnkParent* self, void* arg) {
    u32 result = 0;
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*, void*, u32*)>((*p)->vtable[0x128 / 8])(*p, arg, &result);
    return result;
}
