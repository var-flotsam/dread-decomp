// UnkTU2772 (nn::atk): rounds a signed size up to a multiple of 8. Names and layouts are placeholders.
#include "types.h"

s64 fn_7101212d88(s32 size) {
    return (static_cast<s64>(size) + 7) & ~static_cast<s64>(7);
}
