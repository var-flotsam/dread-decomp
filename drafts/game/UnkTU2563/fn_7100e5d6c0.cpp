// Unit 2563 (CMinimapData hint): equality of two keyed tables (a value list per key): every key of
// one is found in the other and the lists under it hold the same words. Names and layouts are
// placeholders.
#include "types.h"

struct UnkWordList_e5d6c0 {
    u64* data;  // 0x0
    u32 count;  // 0x8
    u8 pad_0xc[0x1c];
};
static_assert(sizeof(UnkWordList_e5d6c0) == 0x28, "placeholder layout");

struct UnkKeyedTable_e5d6c0 {
    UnkWordList_e5d6c0* lists;  // 0x0
    u64* keys;                  // 0x8
    u32 count;                  // 0x10
};

bool fn_7100e5d6c0(UnkKeyedTable_e5d6c0* self, UnkKeyedTable_e5d6c0* other) {
    if (self->count != other->count)
        return false;
    for (u32 i = 0; i < self->count; ++i) {
        u64 key = self->keys[i];
        u64 found = 0;
        while (other->keys[found] != key) {
            if (++found == self->count)
                return false;
        }
        u32 j = static_cast<u32>(found);
        if (j == self->count)
            return false;
        const UnkWordList_e5d6c0& b = other->lists[j];
        const UnkWordList_e5d6c0& a = self->lists[i];
        if (a.count != b.count)
            return false;
        const u64* pb = b.data;
        const u64* pa = a.data;
        for (u32 n = a.count; n; --n, ++pa, ++pb)
            if (*pa != *pb)
                return false;
    }
    return true;
}
