// Copies of fn_7101157b48 (src/game_7100ece7fc/fn_7101157b48.cpp) for other types, with their own callees and data (they name their own callees, data and strings; the comments describe the original): fn_7100030300, fn_710003146c.
// Destructors (deleting, and one complete) of three objects that each hold a list of string-id nodes: restore the vtable,
// release each node's id and free the node through the list's allocator, then free the object.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
struct UnkNodeId {
    UnkStringData* data;

    ~UnkNodeId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdNode_0x18 {
    UnkNodeId id;
    u64 field_0x8;
    UnkIdNode_0x18* next;
};

struct UnkIdNode_0x10 {
    UnkNodeId id;
    UnkIdNode_0x10* next;
};

struct UnkIdNode_0x30 {
    u8 unk_0x0[0x18];
    UnkNodeId id;
    u64 field_0x20;
    UnkIdNode_0x30* next;
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
            n->id.~UnkNodeId();
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

extern void* g_unk_7101a1a0b0[];
extern void* g_unk_7101a1a090[];
extern void* g_unk_7101cb38e8[];
extern void* g_unk_7101cb46a0[];

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

