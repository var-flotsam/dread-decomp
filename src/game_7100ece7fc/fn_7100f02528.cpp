// Destructors (complete and deleting) of two objects that each hold a list of reference nodes
// (keyed in the first, as in fn_7100f00af4's list): restore the vtable, release each node's
// reference and free the node through the list's allocator, then empty the list (the deleting
// ones then free the object).
#include "base/UnkVector.h"
#include "types.h"

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

static inline void RefRelease(UnkRefObj* o) {
    __atomic_fetch_sub(&o->ref_count, 1, __ATOMIC_SEQ_CST);
    if (__atomic_load_n(&o->ref_count, __ATOMIC_SEQ_CST) == 0)
        o->vf_0x10();
}

struct UnkRef {
    UnkRefObj* ptr;
    ~UnkRef() {
        if (ptr)
            RefRelease(ptr);
    }
};

struct UnkKeyRefNode {
    u32 key;
    UnkRef ref;
    UnkKeyRefNode* next;
};

struct UnkRefNode {
    UnkRef ref;
    UnkRefNode* next;
};

template <class Node>
struct UnkList {
    u32 count;
    Node* head;
    Node* tail;
    base::UnkAllocator* allocator;
    u32 flags;

    ~UnkList() {
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

extern void* g_unk_7101caa208[];
extern void* g_unk_7101caa228[];

template <void** Vtable, class Node>
struct UnkRefListOwner {
    void** vtable;
    UnkList<Node> list;

    ~UnkRefListOwner() { vtable = &Vtable[2]; }
};

void fn_7100f02528(UnkRefListOwner<g_unk_7101caa208, UnkKeyRefNode>& self) {
    delete &self;
}

void fn_7100f025c0(UnkRefListOwner<g_unk_7101caa228, UnkRefNode>& self) {
    delete &self;
}

void fn_7100ef36ec(UnkRefListOwner<g_unk_7101caa208, UnkKeyRefNode>& self) {
    self.~UnkRefListOwner();
}

void fn_7100ef364c(UnkRefListOwner<g_unk_7101caa228, UnkRefNode>& self) {
    self.~UnkRefListOwner();
}
