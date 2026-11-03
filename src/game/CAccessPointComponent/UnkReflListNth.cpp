// The element at an index of a linked list whose first node is at +0x40, or null past the end (a
// reflected collection's element access) in the CAccessPointComponent unit; a copy of fn_7100244e90
// (src/base/reflection/UnkReflListNth.cpp). Nodes hold the value at +0x8 and the next node at +0x30.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAccessPointListNode {
    u8 pad_0x0[0x8];
    u8 value[0x28];  // 0x8
    UnkAccessPointListNode* next;  // 0x30
};

struct UnkAccessPointList {
    u8 pad_0x0[0x40];
    UnkAccessPointListNode* head;  // 0x40
};

void* fn_7100a75258(UnkAccessPointList* self, u32 index) {
    UnkAccessPointListNode* node = self->head;
    if (index != 0 && node) {
        do
            node = node->next;
        while (--index != 0 && node);
    }
    return node ? node->value : nullptr;
}
