// The complete destructor of the object whose deleting destructor is fn_7101185ea8 (same unit,
// CCreditsMode::CTunableCredits by tools/tu_map.py): restores the vtable, releases two references,
// destroys a member (fn_710119ad3c) and a list of plain nodes, then runs the base destructor
// (fn_71011867d4). Names and layouts are placeholders, as in fn_7101185ea8.cpp.
#include "base/UnkVector.h"
#include "game/CCreditsMode/CTunableCredits.h"
#include "game/UnkTU2725.h"
#include "game/UnkTU2732.h"
#include "types.h"

struct UnkCreditsRefObj_b44a3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkCreditsRef_b44a3 {
    UnkCreditsRefObj_b44a3* ptr;
    ~UnkCreditsRef_b44a3() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkCreditsBase_b44a3 {
    void** vtable;
    u8 unk_0x8[0x88];
    ~UnkCreditsBase_b44a3() { fn_71011867d4(this); }
};

struct UnkCreditsMember_b44a3 {
    u64 data[0x9a0 / 8];
    ~UnkCreditsMember_b44a3() { fn_710119ad3c(this); }
};

struct UnkCreditsNode_b44a3 {
    u64 field_0x0;
    UnkCreditsNode_b44a3* next;
};

struct UnkCreditsNodeList_b44a3 {
    void** vtable;
    u32 count;
    UnkCreditsNode_b44a3* head;
    UnkCreditsNode_b44a3* tail;
    base::UnkAllocator* allocator;
    u8 unk_0x28[0x18];

    ~UnkCreditsNodeList_b44a3() {
        vtable = &g_unk_7101cb5010[2];
        for (UnkCreditsNode_b44a3* n = head; n;) {
            UnkCreditsNode_b44a3* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkCreditsObject_b44a3 : UnkCreditsBase_b44a3 {
    UnkCreditsNodeList_b44a3 list;
    UnkCreditsMember_b44a3 member;
    UnkCreditsRef_b44a3 field_0xa70;
    UnkCreditsRef_b44a3 field_0xa78;

    ~UnkCreditsObject_b44a3() { vtable = &g_unk_7101cb4ef8[2]; }
};

void fn_7101185dd4(UnkCreditsObject_b44a3* self) {
    self->~UnkCreditsObject_b44a3();
}
