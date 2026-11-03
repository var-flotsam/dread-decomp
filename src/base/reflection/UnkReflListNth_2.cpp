// Copies of fn_7100244e90 (src/base/reflection/UnkReflListNth.cpp): the element at an index of a
// reflected linked list whose first node is at +0x40, or null past the end; nodes hold the value at
// +0x8 and the next node at an offset that depends on the value's size. One copy per list type, in
// units 1721, 2216, 2503, 2518, 2523, 2556, 2599, 2601, 2604, 2605, 2640 and 2648 of tools/tu_map.py.
// Names and layouts are placeholders.
#include "types.h"

template <u32 NextOffset>
struct UnkReflListNode2 {
    u8 pad_0x0[0x8];
    u8 value[NextOffset - 0x8];  // 0x8
    UnkReflListNode2* next;
};

template <u32 NextOffset>
struct UnkReflList2 {
    u8 pad_0x0[0x40];
    UnkReflListNode2<NextOffset>* head;  // 0x40
};

template <u32 NextOffset>
static inline __attribute__((always_inline)) void* ListNth2(UnkReflList2<NextOffset>* self, u32 index) {
    UnkReflListNode2<NextOffset>* node = self->head;
    if (index != 0 && node) {
        do
            node = node->next;
        while (--index != 0 && node);
    }
    return node ? node->value : nullptr;
}

void* fn_7100b80e88(UnkReflList2<0x10>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100caa3b4(UnkReflList2<0x30>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100d41704(UnkReflList2<0x10>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100d6ecf4(UnkReflList2<0x10>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100d6fb78(UnkReflList2<0x10>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100d89c84(UnkReflList2<0x30>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100e0e8f8(UnkReflList2<0x98>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100f79530(UnkReflList2<0x3b0>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100f871e0(UnkReflList2<0x70>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100f920fc(UnkReflList2<0x28>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100f970d4(UnkReflList2<0x80>* self, u32 index) { return ListNth2(self, index); }
void* fn_7100fffa3c(UnkReflList2<0x38>* self, u32 index) { return ListNth2(self, index); }
void* fn_71010035b4(UnkReflList2<0x38>* self, u32 index) { return ListNth2(self, index); }
void* fn_71010048b4(UnkReflList2<0x58>* self, u32 index) { return ListNth2(self, index); }
void* fn_7101005a84(UnkReflList2<0x58>* self, u32 index) { return ListNth2(self, index); }
void* fn_71010425ac(UnkReflList2<0x5d8>* self, u32 index) { return ListNth2(self, index); }
