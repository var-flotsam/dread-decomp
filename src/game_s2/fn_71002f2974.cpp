// Destructor of an object made of a base part (vtable 0x7101a222c8: a list of nodes, which it
// frees through its allocator, at +0x8..) and a derived part (vtable 0x7101a222a8) that also holds
// a second, embedded sub-object (vtable 0x7101a1a4c8 at +0x30, with its own allocator at +0x38).
// The derived part frees the nodes and the sub-object's block, then the base part runs the list
// cleanup again.
#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"

extern void* g_unk_7101a222a8[];  // derived part's vtable
extern void* g_unk_7101a222c8[];  // base part's vtable
extern void* g_unk_7101a1a4c8[];  // the sub-object's vtable

void fn_710007ed40(void* node);

struct UnkListNode {
    u8 pad_0x0[0x68];
    UnkListNode* field_0x68;
};

struct UnkNodeOwner {
    void** vtable;
    u32 field_0x8;
    u32 pad_0xc;
    UnkListNode* field_0x10;
    UnkListNode* field_0x18;
    base::UnkAllocator* field_0x20;
    u8 pad_0x28[8];
    void** field_0x30;
    base::UnkAllocator* field_0x38;
    UnkBlockFreeList field_0x40;  // the sub-object's free list; its buffer is at +0x60
    u8 pad_0x68[4];
    u8 field_0x6c;

    __attribute__((always_inline)) void FreeNodes() {
        for (UnkListNode* node = field_0x10; node;) {
            UnkListNode* next = node->field_0x68;
            fn_710007ed40(node);
            fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
            field_0x20->vf_0x48(node);
            node = next;
        }
    }
};

void fn_71002f2974(UnkNodeOwner* self) {
    self->vtable = g_unk_7101a222a8 + 2;
    self->field_0x6c = 1;
    self->FreeNodes();
    self->field_0x8 = 0;
    self->field_0x10 = nullptr;
    self->field_0x18 = nullptr;
    self->field_0x20 = nullptr;
    self->field_0x30 = g_unk_7101a1a4c8 + 2;
    if (void* block = self->field_0x40.buffer) {
        fn_71000837f4(&self->field_0x40);
        self->field_0x38->vf_0x48(block);
        self->field_0x38 = nullptr;
    }
    self->vtable = g_unk_7101a222c8 + 2;
    self->FreeNodes();
    self->field_0x8 = 0;
    self->field_0x10 = nullptr;
    self->field_0x18 = nullptr;
}
