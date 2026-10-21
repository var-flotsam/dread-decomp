// Rebuilds a 16-bit mask (0x10) with one bit per index the source object reports as set.
#include "types.h"

struct UnkSource_710019ab0c;
bool fn_7100161fb0(UnkSource_710019ab0c* source, s32 index);

struct UnkStruct_710019ab0c {
    u8 unk_0x0[0x8];
    UnkSource_710019ab0c* source;
    u32 mask;
};

void fn_710019ab0c(UnkStruct_710019ab0c* self) {
    self->mask = 0;
    for (s32 i = 0; i < 16; ++i) {
        if (fn_7100161fb0(self->source, i))
            self->mask |= 1 << i;
    }
}
