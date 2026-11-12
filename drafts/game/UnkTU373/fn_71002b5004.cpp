// Unit 373: removes the top entry of a small pointer heap ordered by two floats of each entry.
// Names and layouts are placeholders.
#include "types.h"

struct UnkHeapEntry {
    u8 pad_0x00[0x10];
    f32 field_0x10;
    f32 field_0x14;
};

struct __attribute__((packed)) UnkEntryHeap {
    u32 count;
    UnkHeapEntry* items[1];
};

static inline bool Before(const UnkHeapEntry* a, const UnkHeapEntry* b) {
    f32 sa = a->field_0x10 + a->field_0x14;
    f32 sb = b->field_0x10 + b->field_0x14;
    return sa > sb || (sa == sb && a->field_0x10 < b->field_0x10);
}

extern "C" void* memset(void*, int, unsigned long);

void fn_71002b5004(UnkEntryHeap* heap) {
    s64 n = heap->count;
    UnkHeapEntry** items = heap->items;
    // keeps the count as a full 64-bit value, as the original does
    asm("" : "+r"(n));
    s64 bytes = n * 8;
    // keeps the byte size the original compares against
    asm("" : "+r"(bytes));
    if (bytes > 8) {
        UnkHeapEntry** lastp = (UnkHeapEntry**)((u8*)items + bytes) - 1;
        UnkHeapEntry* top = items[0];
        items[0] = *lastp;
        *lastp = top;
        if (bytes != 16) {
            s64 half = n - 3;
            if (half < 0)
                half = n - 2;
            s64 last = n - 1;
            s64 idx;
            UnkHeapEntry** child;
            if (last < 3) {
                idx = 1;
                child = items + 1;
            } else {
                idx = 1;
                child = items + 1;
                if (Before(items[1], items[2])) {
                    idx = 2;
                    child = items + 2;
                }
            }
            UnkHeapEntry* value = items[0];
            if (!Before(*child, value)) {
                UnkHeapEntry** hole = items;
                for (;;) {
                    UnkHeapEntry** cur = child;
                    *hole = *cur;
                    if ((half >> 1) < idx)
                        break;
                    s64 l = idx * 2 + 1;
                    child = items + l;
                    idx = l;
                    if (l + 1 < last) {
                        if (Before(items[l], items[l + 1])) {
                            child = items + l + 1;
                            idx = l + 1;
                        }
                    }
                    if (Before(*child, value)) {
                        hole = cur;
                        break;
                    }
                    hole = cur;
                }
                *hole = value;
            }
        }
    }
    u32 c = heap->count;
    u32 nc = c - 1;
    if (c == 0) {
        memset(heap->items, 0, (u64)nc * 8);
        heap->count = nc;
        return;
    }
    heap->count = nc;
}
