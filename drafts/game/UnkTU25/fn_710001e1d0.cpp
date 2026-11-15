// Unit 25: compares two arrays (data, count) of 16-byte entries; equal when the counts match and each pair of
// entries has the same two words. Names and layouts are placeholders.
#include "types.h"

struct UnkPair_l01e1d0 {
    u64 first;
    u64 second;
};

struct UnkPairArray_l01e1d0 {
    UnkPair_l01e1d0* data;
    u32 count;
};

bool fn_710001e1d0(const UnkPairArray_l01e1d0* a, const UnkPairArray_l01e1d0* b) {
    u32 n = a->count;
    if (n != b->count)
        return false;
    if (n) {
        const UnkPair_l01e1d0* pa;
        const UnkPair_l01e1d0* pb = b->data;
        pa = a->data;
        u64 left = n;
        do {
            if (pb->second != pa->second || pb->first != pa->first)
                return false;
            --left;
            ++pa;
            ++pb;
        } while (left);
    }
    return true;
}
