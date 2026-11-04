// CVulkranAttackManager::CTunableVulkranAttackManager's unit (tools/tu_map.py): clears the object, as
// fn_71006bd074 (src/game/UnkUnit1055/UnkUnit1055_Clear_b66a4.cpp) does with members 8 bytes further
// on: drops the reference at +0x8, clears the word at +0x10, frees every node of the list at +0x28
// (each node's reference dropped first) through the allocator at +0x38, and resets the count (+0x20)
// and the list. Names and layouts are placeholders.
#include "types.h"

struct UnkRefCounted_b66a4v {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object once the last reference is dropped
    s32 ref_count;           // 0x8
};

struct UnkRefNode_b66a4v {
    UnkRefCounted_b66a4v* obj;
    UnkRefNode_b66a4v* next;  // 0x8
};

struct UnkNodeAllocator_b66a4v {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* p);  // frees
};

struct UnkVulkranRefList_b66a4 {
    u8 pad_0x0[0x8];
    UnkRefCounted_b66a4v* ref;  // 0x8
    u32 field_0x10;
    u8 pad_0x14[0xc];
    u32 count;  // 0x20
    u8 pad_0x24[4];
    UnkRefNode_b66a4v* head;  // 0x28
    UnkRefNode_b66a4v* tail;  // 0x30
    UnkNodeAllocator_b66a4v* allocator;  // 0x38
};

static inline __attribute__((always_inline)) void Release_b66a4v(UnkRefCounted_b66a4v* p) {
    if (--p->ref_count == 0)
        p->vf_0x10();
}

bool fn_71006de888(UnkVulkranRefList_b66a4* self) {
    if (self->ref) {
        Release_b66a4v(self->ref);
        self->ref = nullptr;
    }
    self->field_0x10 = 0;
    for (UnkRefNode_b66a4v* n = self->head; n;) {
        UnkRefNode_b66a4v* cur = n;
        n = n->next;
        if (cur->obj)
            Release_b66a4v(cur->obj);
        self->allocator->vf_0x48(cur);
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    return true;
}
