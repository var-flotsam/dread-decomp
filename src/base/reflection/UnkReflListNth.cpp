// The element at an index of a linked list whose first node is at +0x40, or null past the end (a
// reflected collection's element access). Nodes hold the value at +0x8 and the next node at an
// offset that depends on the value's size. One copy per list type, in units 83 (base::reflection),
// 108, 271 and 341 (CActor) of tools/tu_map.py. Names and layouts are placeholders.
#include "types.h"

template <u32 NextOffset>
struct UnkReflListNode {
    u8 pad_0x0[0x8];
    u8 value[NextOffset - 0x8];  // 0x8
    UnkReflListNode* next;
};

template <u32 NextOffset>
struct UnkReflList {
    u8 pad_0x0[0x40];
    UnkReflListNode<NextOffset>* head;  // 0x40
};

template <u32 NextOffset>
static inline __attribute__((always_inline)) void* ListNth(UnkReflList<NextOffset>* self, u32 index) {
    UnkReflListNode<NextOffset>* node = self->head;
    if (index != 0 && node) {
        do
            node = node->next;
        while (--index != 0 && node);
    }
    return node ? node->value : nullptr;
}

void* fn_71000d6a20(UnkReflList<0x20>* self, u32 index) { return ListNth(self, index); }
void* fn_7100121108(UnkReflList<0x10>* self, u32 index) { return ListNth(self, index); }
void* fn_71001f2344(UnkReflList<0x10>* self, u32 index) { return ListNth(self, index); }
void* fn_7100242e98(UnkReflList<0x10>* self, u32 index) { return ListNth(self, index); }
void* fn_7100244e90(UnkReflList<0x30>* self, u32 index) { return ListNth(self, index); }
void* fn_7100248420(UnkReflList<0x30>* self, u32 index) { return ListNth(self, index); }
