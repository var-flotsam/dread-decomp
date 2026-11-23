// Calls virtual slot 0x48 on each child object of the array at +0xa0.
#include "game_s2/UnkChild.h"
#include "types.h"

struct UnkParent {
    u8 pad_0x0[0xa0];
    UnkChild** children;
    u32 count;
};

void fn_71002d5658(UnkParent* self) {
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p)
        reinterpret_cast<void (*)(UnkChild*)>((*p)->vtable[0x48 / 8])(*p);
}
