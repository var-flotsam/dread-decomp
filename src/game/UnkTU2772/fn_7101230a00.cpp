#include "types.h"

// NON_MATCHING(minor): the same instructions in another order or with other registers.
u32 fn_7101230a00(u8* self) {
    u32* p = (u32*)(self + 0x0);
    u32 f = *p;
    if (!(f & 4))
        return 0;
    u32 idx = (f & 1) + ((f >> 1) & 1);
    return ((u16*)(p + 1 + idx))[1];
}
