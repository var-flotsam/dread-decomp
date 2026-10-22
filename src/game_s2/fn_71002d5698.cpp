// Runs fn_71002df9f4, clears +0x100, then calls virtual slot 0x50 on each child object of the array at +0xa0.
#include "types.h"

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0xa0];
    UnkChild** children;
    u32 count;
    u8 pad_0xac[0x54];
    u32 field_0x100;
};

void fn_71002df9f4(UnkParent*);

void fn_71002d5698(UnkParent* self) {
    fn_71002df9f4(self);
    u32 n = self->count;
    self->field_0x100 = 0;
    UnkChild** end = self->children + n;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*)>((*p)->vtable[0x50 / 8])(*p);
}
