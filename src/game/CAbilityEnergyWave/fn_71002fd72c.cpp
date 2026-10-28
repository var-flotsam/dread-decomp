// Runs fn_71010c4228, then calls virtual slot 0x38 on each child object of the array at +0x40.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_71010c4228(UnkParent*);

void fn_71002fd72c(UnkParent* self) {
    fn_71010c4228(self);
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*)>((*p)->vtable[0x38 / 8])(*p);
}
