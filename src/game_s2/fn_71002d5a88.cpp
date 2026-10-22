// Calls virtual slot 0x30 on each child object of the array at +0xc8, passing `arg` through.
#include "types.h"

struct UnkChild {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(void* arg);
};

struct UnkParent {
    u8 pad_0x0[0xc8];
    UnkChild** children;
    u8 pad_0xd0[8];
    u32 count;
};

void fn_71002d5a88(UnkParent* self, void* arg) {
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        (*p)->vf_0x30(arg);
}
