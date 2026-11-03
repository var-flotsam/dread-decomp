// Unit 1629 (tools/tu_map.py; no reflected class): fn_7100ad1e5c compares two arrays (data, count) of
// 0x80-byte entries by the word at +0x8 of each: equal when the counts match and every pair does.
// Names and layouts are placeholders.
#include "types.h"

struct UnkKeyedEntry_b49 {
    u8 pad_0x0[0x8];
    u64 key;
    u8 pad_0x10[0x70];
};

struct UnkKeyedArray_b49 {
    UnkKeyedEntry_b49* data;
    u32 count;
};

bool fn_7100ad1e5c(const UnkKeyedArray_b49* a, const UnkKeyedArray_b49* b) {
    u32 n = a->count;
    if (n != b->count)
        return false;
    if (n) {
        const UnkKeyedEntry_b49* pa;
        const UnkKeyedEntry_b49* pb = b->data;
        pa = a->data;
        u64 left = n;
        do {
            if (pb->key != pa->key)
                return false;
            --left;
            ++pa;
            ++pb;
        } while (left);
    }
    return true;
}
