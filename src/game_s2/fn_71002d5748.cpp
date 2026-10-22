// Forwards a copy of *value and `flag` to fn_710070d750 for the object at +0x70 of each child in the array at +0xc8.
#include "types.h"

struct UnkChild {
    u8 pad_0x0[0x70];
    void* field_0x70;
};

struct UnkParent {
    u8 pad_0x0[0xc8];
    UnkChild** children;
    u8 pad_0xd0[8];
    u32 count;
};

void fn_710070d750(void* obj, u32* value, bool flag);

void fn_71002d5748(UnkParent* self, const u32* value, bool flag) {
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p) {
        void* obj = (*p)->field_0x70;
        alignas(8) u32 v = *value;
        fn_710070d750(obj, &v, flag);
    }
}
