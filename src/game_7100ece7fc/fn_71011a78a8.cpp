// Destructors (complete and deleting) of three objects that each hold a list of reference nodes:
// restore the vtable, release each node's reference and free the node through the list's
// allocator, then empty the list (the deleting ones free the object instead).
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkRef {
    UnkRefObj* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkRefNode_0x18 {
    UnkRef ref;
    u64 field_0x8;
    UnkRefNode_0x18* next;
};

struct UnkRefNode_0x10 {
    UnkRef ref;
    UnkRefNode_0x10* next;
};

// Pad: the bytes between the tail and the allocator.
template <class Node, u32 Pad>
struct UnkRefList {
    u32 count;
    Node* head;
    Node* tail;
    u8 unk_0x18[Pad];
    base::UnkAllocator* allocator;

    ~UnkRefList() {
        for (Node* n = head; n;) {
            Node* next = n->next;
            n->ref.~UnkRef();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

extern void* g_unk_7101cb5608[];
extern void* g_unk_7101cb5668[];
extern void* g_unk_7101cb58e8[];

template <void** Vtable, class Node, u32 Pad>
struct UnkRefListOwner {
    void** vtable;
    UnkRefList<Node, Pad> list;

    ~UnkRefListOwner() { vtable = &Vtable[2]; }
};

using UnkOwnerA = UnkRefListOwner<g_unk_7101cb5608, UnkRefNode_0x18, 0>;
using UnkOwnerB = UnkRefListOwner<g_unk_7101cb5668, UnkRefNode_0x18, 8>;
using UnkOwnerC = UnkRefListOwner<g_unk_7101cb58e8, UnkRefNode_0x10, 8>;

void fn_71011a78a8(UnkOwnerA& self) {
    self.~UnkOwnerA();
}

void fn_71011a793c(UnkOwnerA& self) {
    delete &self;
}

void fn_71011a7b90(UnkOwnerB& self) {
    self.~UnkOwnerB();
}

void fn_71011a7c24(UnkOwnerB& self) {
    delete &self;
}

void fn_71011bbd2c(UnkOwnerC& self) {
    self.~UnkOwnerC();
}

void fn_71011bbdc0(UnkOwnerC& self) {
    delete &self;
}
