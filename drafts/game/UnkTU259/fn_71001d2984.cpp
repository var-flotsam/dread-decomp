// Unit 259: sorts the entries [lo, hi] of the pointer array at +0xc0140 by their key (+0x28), ascending:
// a quicksort that finishes ranges of up to five entries with a bubble sort.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSortEntry_l1d2984 {
    u8 pad_0x0[0x28];
    u32 key;  // 0x28
};

struct UnkSortOwner_l1d2984 {
    u8 pad_0x0[0xc0140];
    UnkSortEntry_l1d2984* entries[1];  // 0xc0140; the count is not known
};

void fn_71001d2984(UnkSortOwner_l1d2984* self, s32 lo, s32 hi) {
    if (lo >= hi)
        return;
    UnkSortEntry_l1d2984** entries = self->entries;
    s32 count = hi - lo;
    if (count > 4) {
        UnkSortEntry_l1d2984* pivot = entries[(lo + hi) / 2];
        s32 j = hi;
        s32 i = lo;
        do {
            while (entries[i]->key < pivot->key)
                i++;
            while (entries[j]->key > pivot->key)
                j--;
            if (i <= j) {
                UnkSortEntry_l1d2984* tmp = entries[i];
                entries[i] = entries[j];
                entries[j] = tmp;
                i++;
                j--;
            }
        } while (i <= j);
        if (lo < j)
            fn_71001d2984(self, lo, j);
        if (i < hi)
            fn_71001d2984(self, i, hi);
        return;
    }
    for (s32 pass = 0; pass < count; pass++) {
        for (s32 k = 0; k < count - pass; k++) {
            if (entries[lo + k]->key > entries[lo + k + 1]->key) {
                UnkSortEntry_l1d2984* tmp = entries[lo + k];
                entries[lo + k] = entries[lo + k + 1];
                entries[lo + k + 1] = tmp;
            }
        }
    }
}
