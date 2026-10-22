// Unless a global flag is set, calls virtual slot 0x100 on each child object of the array at +0x40.
#include "types.h"

struct UnkGlobals {
    u8 pad_0x0[0x29c0];
    u8 field_0x29c0;
};

extern UnkGlobals* g_unk_7101d47310;

struct UnkChild {
    void** vtable;
};

struct UnkParent {
    u8 pad_0x0[0x40];
    UnkChild** children;
    u32 count;
};

void fn_71002fe470(UnkParent* self) {
    if (g_unk_7101d47310->field_0x29c0)
        return;
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*)>((*p)->vtable[0x100 / 8])(*p);
}
