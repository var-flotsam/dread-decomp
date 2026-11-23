// Destructors (deleting, and one complete) of three objects that each hold a list of string-id nodes: restore the vtable,
// release each node's id and free the node through the list's allocator, then free the object.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CFXComponent.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkIdNode_0x10.h"
#include "game/UnkIdNode_0x18.h"
#include "game/UnkIdNode_0x30.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// Pad: the bytes between the tail and the allocator.
template <class Node, u32 Pad>
struct UnkIdList {
    u32 count;
    Node* head;
    Node* tail;
    u8 unk_0x18[Pad];
    base::UnkAllocator* allocator;

    ~UnkIdList() {
        for (Node* n = head; n;) {
            Node* next = n->next;
            n->id.~UnkStrIdTwoStep();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

extern void* g_unk_7101cb38a8[];

template <void** Vtable, class Node, u32 Pad>
struct UnkIdListOwner {
    void** vtable;
    UnkIdList<Node, Pad> list;

    ~UnkIdListOwner() { vtable = &Vtable[2]; }
};

void fn_7101157b48(UnkIdListOwner<g_unk_7101cb38a8, UnkIdNode_0x18, 0>& self) {
    delete &self;
}

void fn_71011581a4(UnkIdListOwner<g_unk_7101cb38e8, UnkIdNode_0x10, 8>& self) {
    delete &self;
}

void fn_71011580d4(UnkIdListOwner<g_unk_7101cb38e8, UnkIdNode_0x10, 8>& self) {
    self.~UnkIdListOwner();
}

void fn_7101174a64(UnkIdListOwner<g_unk_7101cb46a0, UnkIdNode_0x30, 0>& self) {
    delete &self;
}

// The same destructors in other units, with their own vtables and the allocator right after the tail.
void fn_7100de8d24(UnkIdListOwner<g_unk_7101aa9ae0, UnkIdNode_0x10, 0>& self) {
    self.~UnkIdListOwner();
}

void fn_7100e05e44(UnkIdListOwner<g_unk_7101aa9ae0, UnkIdNode_0x10, 0>& self) {
    delete &self;
}

void fn_710114e408(UnkIdListOwner<g_unk_7101cb38a8, UnkIdNode_0x18, 0>& self) {
    self.~UnkIdListOwner();
}

// The complete destructor of fn_7101174a64's class, in another unit.
void fn_7101171378(UnkIdListOwner<g_unk_7101cb46a0, UnkIdNode_0x30, 0>& self) {
    self.~UnkIdListOwner();
}
