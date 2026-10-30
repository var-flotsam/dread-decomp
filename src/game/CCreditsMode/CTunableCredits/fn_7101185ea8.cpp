// Deleting destructor of a large object: restores the vtable, releases two references, destroys
// a member (fn_710119ad3c) and a list of plain nodes, runs the base destructor (fn_71011867d4)
// and frees the object.
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

struct UnkBase_0x90 {
    void** vtable;
    u8 unk_0x8[0x88];
    ~UnkBase_0x90() asm("fn_71011867d4");  // fn_71011867d4
};

struct UnkMember_0x9a0 {
    u64 data[0x9a0 / 8];
    ~UnkMember_0x9a0() asm("fn_710119ad3c");  // fn_710119ad3c
};

struct UnkNode {
    u64 field_0x0;
    UnkNode* next;
};

extern void* g_unk_7101cb4ef8[];
extern void* g_unk_7101cb5010[];

struct UnkNodeList_0x40 {
    void** vtable;
    u32 count;
    UnkNode* head;
    UnkNode* tail;
    base::UnkAllocator* allocator;
    u8 unk_0x28[0x18];

    ~UnkNodeList_0x40() {
        vtable = &g_unk_7101cb5010[2];
        for (UnkNode* n = head; n;) {
            UnkNode* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkObject_7101185ea8 : UnkBase_0x90 {
    UnkNodeList_0x40 list;
    UnkMember_0x9a0 member;
    UnkRef field_0xa70;
    UnkRef field_0xa78;

    ~UnkObject_7101185ea8() { vtable = &g_unk_7101cb4ef8[2]; }
};

void fn_7101185ea8(UnkObject_7101185ea8& self) {
    delete &self;
}
