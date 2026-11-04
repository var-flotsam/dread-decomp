// GUI::CSamusMenuComposition's unit (1714): the destructor of a string list with an embedded sub-object, the
// shape of base/UnkNodeOwner.h (fn_71010f9550) with a word between the tail and the allocator (the list of
// fn_7100b6fe14, src/game_s1/fn_7100219134.cpp): nodes linked at 0x30 whose strings are destroyed
// (fn_710007ecd4), the sub-object's block freed, then the base part (vtable unk_7101a9e948) runs the list cleanup
// again. Names and layouts are placeholders.
#include "base/UnkNodeOwner.h"

extern void* g_unk_7101a9e928[];  // derived part's vtable
extern char unk_7101a9e948[];     // base part's vtable (src/game_s1/fn_7100219134.cpp)

struct UnkStrListOwner_A6_b63 {
    void** vtable;
    u32 count;
    u32 pad_0xc;
    UnkListNode<0x30>* head;  // 0x10
    UnkListNode<0x30>* tail;
    u64 field_0x20;
    base::UnkAllocator* allocator;  // 0x28
    u8 pad_0x30[8];
    void* field_0x38;
    base::UnkAllocator* field_0x40;
    UnkBlockFreeList field_0x48;  // the sub-object's free list; its buffer is at +0x68
    u8 pad_0x70[4];
    u8 field_0x74;

    __attribute__((always_inline)) void FreeNodes() {
        for (UnkListNode<0x30>* node = head; node;) {
            UnkListNode<0x30>* next = node->next;
            UnkDestroyStrNodeOnly<0x30>(node);
            allocator->vf_0x48(node);
            node = next;
        }
    }
};

void fn_7100b6f3c0(UnkStrListOwner_A6_b63* self) {
    self->vtable = g_unk_7101a9e928 + 2;
    self->field_0x74 = 1;
    self->FreeNodes();
    self->count = 0;
    __builtin_memset(&self->head, 0, 16);
    self->allocator = nullptr;
    self->field_0x38 = g_unk_7101a1a4c8 + 0x10;
    if (void* block = self->field_0x48.buffer) {
        fn_71000837f4(&self->field_0x48);
        self->field_0x40->vf_0x48(block);
        self->field_0x40 = nullptr;
    }
    self->vtable = reinterpret_cast<void**>(unk_7101a9e948) + 2;
    self->FreeNodes();
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
