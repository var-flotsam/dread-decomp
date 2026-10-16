// Element-wise equality of two arrays of 64-bit values. The original walks both
// arrays with a 64-bit down-counter and loads b's element before a's.
#include "types.h"

struct UnkArray_u64 {
    u64* data;
    u32 count;
};

bool fn_710061a210(const UnkArray_u64* a, const UnkArray_u64* b) {
    if (a->count != b->count)
        return false;
    const u64* pa = a->data;
    const u64* pb = b->data;
    for (u64 n = a->count; n != 0; --n, ++pa, ++pb) {
        if (*pb != *pa)
            return false;
    }
    return true;
}
