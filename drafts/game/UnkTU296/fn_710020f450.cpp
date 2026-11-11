// Unit 296: sorts a range of pointers (lo to hi, inclusive) in descending order of the key at +0xd0
// of the objects, storing each object's new position in +0xd8: a quicksort around the middle key,
// and a bubble sort for ranges of at most five. Names and layouts are placeholders.
#include "types.h"

struct UnkSorted_ovb0262t {
    u8 pad_0x0[0xd0];
    s32 key;  // 0xd0
    u8 pad_0xd4[4];
    // A union member: the index writes are untyped, so the pointers are read again after them.
    union {
        s32 index;  // 0xd8
    };
};

void fn_710020f450(s32 lo, s32 hi, UnkSorted_ovb0262t** items) {
    if (lo < hi) {
        if (hi - lo > 4) {
            s32 pivot = items[(lo + hi) / 2]->key;
            s32 i = lo;
            s32 j = hi;
            do {
                while (items[i]->key > pivot)
                    ++i;
                while (items[j]->key < pivot)
                    --j;
                if (i <= j) {
                    UnkSorted_ovb0262t* a = items[i];
                    UnkSorted_ovb0262t* b = items[j];
                    items[i] = b;
                    items[j] = a;
                    items[i]->index = i;
                    items[j]->index = j;
                    ++i;
                    --j;
                }
            } while (i <= j);
            if (j > lo)
                fn_710020f450(lo, j, items);
            if (i < hi)
                fn_710020f450(i, hi, items);
        } else {
            s32 n = hi - lo;
            for (s32 pass = 0; pass < n; ++pass) {
                for (s32 k = 0; k < n - pass; ++k) {
                    UnkSorted_ovb0262t* a = items[lo + k];
                    UnkSorted_ovb0262t* b = items[lo + k + 1];
                    if (a->key < b->key) {
                        items[lo + k] = b;
                        items[lo + k + 1] = a;
                        items[lo + k]->index = lo + k;
                        items[lo + k + 1]->index = lo + k + 1;
                    }
                }
            }
        }
    }
}
