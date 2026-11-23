// The largest value returned by virtual slot 0x80 of the child objects in the array at +0x38 (0 when empty).
#include "game_s2/UnkChild.h"
#include "types.h"

struct UnkParent {
    u8 pad_0x0[0x38];
    UnkChild** children;
    u32 count;
};

s32 fn_71002dfb0c(UnkParent* self) {
    s32 best = 0;
    UnkChild** end = self->children + self->count;
    for (UnkChild** p = self->children; p != end; ++p) {
        s32 v = reinterpret_cast<s32 (*)(UnkChild*)>((*p)->vtable[0x80 / 8])(*p);
        best = best > v ? best : v;
    }
    return best;
}
