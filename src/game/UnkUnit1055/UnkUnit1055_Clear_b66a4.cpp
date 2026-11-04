// Unit 1055 of tools/tu_map.py (no class name): clears the object: drops the reference at +0x8,
// then frees every node of the list at +0x20 (each node's reference dropped first) through the
// allocator at +0x30, and resets the count and the list. Names and layouts are placeholders.
#include "types.h"

struct UnkRefCounted_b66a4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object once the last reference is dropped
    s32 ref_count;           // 0x8
};

struct UnkRefNode_b66a4 {
    UnkRefCounted_b66a4* obj;
    UnkRefNode_b66a4* next;  // 0x8
};

struct UnkNodeAllocator_b66a4 {
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

struct UnkUnit1055Object_b66a4 {
    u8 pad_0x0[0x8];
    UnkRefCounted_b66a4* ref;  // 0x8
    u8 pad_0x10[0x8];
    u32 count;  // 0x18
    u8 pad_0x1c[4];
    UnkRefNode_b66a4* head;  // 0x20
    UnkRefNode_b66a4* tail;  // 0x28
    UnkNodeAllocator_b66a4* allocator;  // 0x30
};

static inline __attribute__((always_inline)) void Release_b66a4(UnkRefCounted_b66a4* p) {
    if (--p->ref_count == 0)
        p->vf_0x10();
}

bool fn_71006bd074(UnkUnit1055Object_b66a4* self) {
    if (self->ref) {
        Release_b66a4(self->ref);
        self->ref = nullptr;
    }
    for (UnkRefNode_b66a4* n = self->head; n;) {
        UnkRefNode_b66a4* cur = n;
        n = n->next;
        if (cur->obj)
            Release_b66a4(cur->obj);
        self->allocator->vf_0x48(cur);
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    return true;
}
