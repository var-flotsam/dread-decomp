#pragma once

// An object made of a base part (a list of nodes, which it frees through its allocator, at +0x8..)
// and a derived part that also holds a second, embedded sub-object (vtable 0x7101a1a4c8 at +0x30,
// with its own allocator at +0x38). Its destructor (fn_71002f2974,
// src/game/CPlaythroughDef/fn_71002f2974.cpp, and the copies for other node types) frees the nodes
// and the sub-object's block, then the base part runs the list cleanup again. Each user names its
// node type (the offset of the next link) and the two vtables. Names and layouts are placeholders.

#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"

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
    void* field_0x30;
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
        field_0x30 = g_unk_7101a1a4c8 + 0x10;
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

// A node that starts with a string: its text is freed (fn_710007ed40), then the string destroyed.
template <u32 Next>
__attribute__((always_inline)) inline void UnkDestroyStrNode(UnkListNode<Next>* node) {
    auto* str = reinterpret_cast<base::global::CRntString*>(node);
    fn_710007ed40(str);
    fn_710007ecd4(str);
}

// A node whose string is only destroyed.
template <u32 Next>
__attribute__((always_inline)) inline void UnkDestroyStrNodeOnly(UnkListNode<Next>* node) {
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(node));
}

// The string list fn_71000732d4 destroys (src/game_s1/fn_71000fc7cc.cpp): next link at +0x30; its
// derived part's vtable is g_unk_7101a1a4a8.
extern void* g_unk_7101a1a4a8[];
using UnkStrList30 = UnkNodeOwner<UnkListNode<0x30>, UnkDestroyStrNodeOnly<0x30>>;
static_assert(sizeof(UnkStrList30) == 0x70, "UnkNodeOwner placeholder layout");
void fn_71000732d4(UnkStrList30* self);
