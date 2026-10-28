// Copies of fn_71011580d4 (src/game/CFXComponent/fn_7101157b48.cpp) for other types, naming their own callees and data
// (the comments are adapted from the original's): fn_710002f83c, fn_7100030c10, fn_710006c364, fn_7100087cf8; and of
// fn_7100030300 (src/game_s7/fn_7100030300.cpp): fn_710006c974, fn_710008883c. Destructors (complete, and two deleting)
// of objects that hold a list of string-id nodes: restore the vtable, release each node's id and free the node through
// the list's allocator.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
template <UnkStringPool*& Pool>
struct UnkNodeId {
    UnkStringData* data;

    ~UnkNodeId() {
        if (UnkStringPool* pool = Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

// Gap: the bytes between the id and the next pointer.
template <UnkStringPool*& Pool, u32 Gap>
struct UnkIdNode {
    UnkNodeId<Pool> id;
    u8 unk_0x8[Gap];
    UnkIdNode* next;
};

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
            n->id.~decltype(n->id)();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

extern void* g_unk_7101a1a090[];
extern void* g_unk_7101a1a0b0[];
extern void* g_unk_7101a1a160[];
extern void* g_unk_7101a1b030[];

template <void** Vtable, class Node, u32 Pad>
struct UnkIdListOwner {
    void** vtable;
    UnkIdList<Node, Pad> list;

    ~UnkIdListOwner() { vtable = &Vtable[2]; }
};


void fn_710002f83c(UnkIdListOwner<g_unk_7101a1a090, UnkIdNode<g_unk_7101d47108, 8>, 0>& self) {
    self.~UnkIdListOwner();
}

void fn_7100030c10(UnkIdListOwner<g_unk_7101a1a0b0, UnkIdNode<g_unk_7101d47108, 8>, 0>& self) {
    self.~UnkIdListOwner();
}

void fn_710006c364(UnkIdListOwner<g_unk_7101a1a160, UnkIdNode<g_unk_7101d47108, 0x10>, 0>& self) {
    self.~UnkIdListOwner();
}

void fn_7100087cf8(UnkIdListOwner<g_unk_7101a1b030, UnkIdNode<g_unk_7101d47200, 0>, 0>& self) {
    self.~UnkIdListOwner();
}

// Deleting destructors (copies of fn_7100030300 in src/game_s7/fn_7100030300.cpp).
void fn_710006c974(UnkIdListOwner<g_unk_7101a1a160, UnkIdNode<g_unk_7101d47108, 0x10>, 0>& self) {
    delete &self;
}

void fn_710008883c(UnkIdListOwner<g_unk_7101a1b030, UnkIdNode<g_unk_7101d47200, 0>, 0>& self) {
    delete &self;
}
