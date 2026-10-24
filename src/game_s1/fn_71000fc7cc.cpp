// Copies of fn_71002f2974 (src/game_s2/fn_71002f2974.cpp) for other node types, naming their own callees and data (the
// comments are adapted from the original's): fn_71000fc7cc (nodes linked at 0x148), fn_71000732d4 (nodes linked at 0x30
// and destroyed by fn_710007ecd4 alone). Destructor of an object made of a base part (a list of nodes, which it frees
// through its allocator, at +0x8..) and a derived part that also holds a second, embedded sub-object (vtable
// 0x7101a1a4c8 at +0x30, with its own allocator at +0x38). The derived part frees the nodes and the sub-object's block,
// then the base part runs the list cleanup again.
#include "base/UnkBlockFreeList.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"

extern void* g_unk_7101a1bcd0[];  // derived part's vtable (fn_71000fc7cc)
extern void* g_unk_7101a1bcf0[];  // base part's vtable (fn_71000fc7cc)
extern void* g_unk_7101a1a4a8[];  // derived part's vtable (fn_71000732d4)
extern void* g_unk_7101a1a070[];  // base part's vtable (fn_71000732d4)
extern void* g_unk_7101a1a4c8[];  // the sub-object's vtable

void fn_710007ed40(void* node);

template <u32 Next>
struct UnkListNode {
    u8 pad_0x0[Next];
    UnkListNode* next;
};

template <class Node, void (*DestroyNode)(Node*)>
struct UnkNodeOwner {
    void** vtable;
    u32 field_0x8;
    u32 pad_0xc;
    Node* field_0x10;
    Node* field_0x18;
    base::UnkAllocator* field_0x20;
    u8 pad_0x28[8];
    void** field_0x30;
    base::UnkAllocator* field_0x38;
    UnkBlockFreeList field_0x40;  // the sub-object's free list; its buffer is at +0x60
    u8 pad_0x68[4];
    u8 field_0x6c;

    __attribute__((always_inline)) void FreeNodes() {
        for (Node* node = field_0x10; node;) {
            Node* next = node->next;
            DestroyNode(node);
            field_0x20->vf_0x48(node);
            node = next;
        }
    }

    __attribute__((always_inline)) void Destroy(void** derived_vtable, void** base_vtable) {
        vtable = derived_vtable + 2;
        field_0x6c = 1;
        FreeNodes();
        field_0x8 = 0;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = nullptr;
        field_0x30 = g_unk_7101a1a4c8 + 2;
        if (void* block = field_0x40.buffer) {
            fn_71000837f4(&field_0x40);
            field_0x38->vf_0x48(block);
            field_0x38 = nullptr;
        }
        vtable = base_vtable + 2;
        FreeNodes();
        field_0x8 = 0;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
    }
};

__attribute__((always_inline)) inline void DestroyNode148(UnkListNode<0x148>* node) {
    fn_710007ed40(node);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
}

__attribute__((always_inline)) inline void DestroyNode30(UnkListNode<0x30>* node) {
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
}

void fn_71000fc7cc(UnkNodeOwner<UnkListNode<0x148>, DestroyNode148>* self) {
    self->Destroy(g_unk_7101a1bcd0, g_unk_7101a1bcf0);
}

void fn_71000732d4(UnkNodeOwner<UnkListNode<0x30>, DestroyNode30>* self) {
    self->Destroy(g_unk_7101a1a4a8, g_unk_7101a1a070);
}
