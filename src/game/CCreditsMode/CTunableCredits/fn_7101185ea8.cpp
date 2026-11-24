// Deleting destructor of a large object: restores the vtable, releases two references, destroys
// a member (fn_710119ad3c) and a list of plain nodes, runs the base destructor (fn_71011867d4)
// and frees the object.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CCreditsMode/CTunableCredits.h"
#include "types.h"

struct UnkRefObj_01185ea8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkRef_01185ea8 {
    UnkRefObj_01185ea8* ptr;
    ~UnkRef_01185ea8() {
        if (auto* held = ptr; held && --held->ref_count == 0)
            held->vf_0x10();
    }
};

struct UnkBase_0x90 {
    void** vtable;
    u8 unk_0x8[0x88];
    ~UnkBase_0x90();  // fn_71011867d4
};

struct UnkMember_0x9a0 {
    u64 data[0x9a0 / 8];
    ~UnkMember_0x9a0();  // fn_710119ad3c
};

struct UnkNode_01185ea8 {
    u64 field_0x0;
    UnkNode_01185ea8* next;
};

struct UnkNodeList_0x40 {
    void** vtable;
    u32 count;
    UnkNode_01185ea8* head;
    UnkNode_01185ea8* tail;
    base::IAllocator* allocator;
    u8 unk_0x28[0x18];

    ~UnkNodeList_0x40() {
        vtable = &g_unk_7101cb5010[2];
        for (UnkNode_01185ea8* n = head; n;) {
            UnkNode_01185ea8* next = n->next;
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
    UnkRef_01185ea8 field_0xa70;
    UnkRef_01185ea8 field_0xa78;

    ~UnkObject_7101185ea8() { vtable = &g_unk_7101cb4ef8[2]; }
};

void CCreditsMode_DestroyAndDelete(UnkObject_7101185ea8& self) {
    delete &self;
}
