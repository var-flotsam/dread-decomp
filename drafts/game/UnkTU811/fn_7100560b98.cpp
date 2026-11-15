// Unit 811 (CEmmyAIComponent hint): removes the top of a heap of pointers kept in an array that follows a
// 32-bit count (a priority queue ordered by the sum of two floats at +0x18 of each element), the sift-down
// written as libc++'s pop_heap does. Names and layouts are placeholders.
#include <string.h>

#include "types.h"

struct UnkEntry_560b98 {
    u8 pad_0x0[0x18];
    f32 a;  // 0x18
    f32 b;  // 0x1c
};

#pragma pack(push, 4)
struct UnkHeap_560b98 {
    u32 count;
    UnkEntry_560b98* items[1];  // 0x4
};
#pragma pack(pop)

static inline bool Greater(const UnkEntry_560b98* x, const UnkEntry_560b98* y) {
    return x->a + x->b > y->a + y->b;
}

void fn_7100560b98(UnkHeap_560b98* self) {
    UnkEntry_560b98** first = self->items;
    UnkEntry_560b98** last = first + self->count;
    if (last - first > 1) {
        UnkEntry_560b98* top = *first;
        *first = *(last - 1);
        *(last - 1) = top;
        --last;
        s64 len = last - first;
        if (len > 1) {
            s64 limit = (len - 2) / 2;
            s64 child = 1;
            UnkEntry_560b98** childIt = first + 1;
            if (child + 1 < len && Greater(childIt[0], childIt[1])) {
                ++childIt;
                ++child;
            }
            if (!Greater(*childIt, *first)) {
                UnkEntry_560b98* value = *first;
                UnkEntry_560b98** hole = first;
                do {
                    *hole = *childIt;
                    hole = childIt;
                    if (limit < child)
                        break;
                    child = 2 * child + 1;
                    childIt = first + child;
                    if (child + 1 < len && Greater(childIt[0], childIt[1])) {
                        ++childIt;
                        ++child;
                    }
                } while (!Greater(*childIt, value));
                *hole = value;
            }
        }
    }
    u32 newCount = self->count - 1;
    if (self->count == 0)
        memset(self->items, 0, newCount * sizeof(UnkEntry_560b98*));
    self->count = newCount;
}
