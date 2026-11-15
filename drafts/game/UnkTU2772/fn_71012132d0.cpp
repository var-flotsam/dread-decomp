// UnkTU2772 (nn::atk): clamps a byte-sized enum value to at most 2. Names and layouts are placeholders.
#include "types.h"

u32 fn_71012132d0(u8 value) {
    if (value > 2)
        return 2;
    return value;
}
