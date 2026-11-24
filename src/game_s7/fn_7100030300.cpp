// Copies of fn_7101157b48 (src/game/CFXComponent/fn_7101157b48.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100030300, fn_710003146c.
// Destructors (deleting, and one complete) of three objects that each hold a list of string-id nodes: restore the vtable,
// release each node's id and free the node through the list's allocator, then free the object.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CFXComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkIdNode_0x10.h"
#include "game/UnkIdNode_0x18.h"
#include "game/UnkIdNode_0x30.h"
#include "game/UnkTU39.h"
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
    base::IAllocator* allocator;

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

template <void** Vtable, class Node, u32 Pad>
struct UnkIdListOwner {
    void** vtable;
    UnkIdList<Node, Pad> list;

    ~UnkIdListOwner() { vtable = &Vtable[2]; }
};

void fn_7100030300(UnkIdListOwner<g_unk_7101a1a090, UnkIdNode_0x18, 0>& self) {
    delete &self;
}

void fn_710003146c(UnkIdListOwner<g_unk_7101a1a0b0, UnkIdNode_0x18, 0>& self) {
    delete &self;
}
