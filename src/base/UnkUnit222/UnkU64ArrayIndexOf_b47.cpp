// Unit 222 (tools/tu_map.py): the index of a 64-bit key in an array of keys (data +0x10, count
// +0x18), or -1. Names and layout are placeholders.
#include "types.h"

struct UnkU64Array_b47 {
    u8 pad_0x0[0x10];
    u64* data;  // 0x10
    u32 count;  // 0x18
};

s32 fn_710019ba50(UnkU64Array_b47* self, const u64* key) {
    u32 n = self->count;
    if (n == 0)
        return -1;
    u64* begin = self->data;
    u64 k = *key;
    u64* it = begin;
    if (*it != k) {
        u64* end = begin + n;
        ++it;
        for (;;) {
            if (end <= it)
                return -1;
            if (*it++ == k)
                break;
        }
        --it;
    }
    return (s32)(it - begin);
}
