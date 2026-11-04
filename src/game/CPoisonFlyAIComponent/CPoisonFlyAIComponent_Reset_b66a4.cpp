// Unit 958 of tools/tu_map.py (CPoisonFlyAIComponent's neighbourhood, no name hint): a reset. Frees
// every node of the list at +0x40 (each node's reference dropped first) through the allocator at
// +0x50 and clears the list and its count (+0x38), drops the reference at +0xa0, and takes the
// tunable's word at +0x4c (CTunableCharClassPoisonFlyAIComponent, g_unk_71726fa0f8) into +0xb4.
// Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "types.h"

extern UnkTunable g_unk_71726fa0f8;

struct UnkRefCounted_b66a4p {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object once the last reference is dropped
    s32 ref_count;           // 0x8
};

struct UnkRefNode_b66a4p {
    UnkRefCounted_b66a4p* obj;
    UnkRefNode_b66a4p* next;  // 0x8
};

struct UnkNodeAllocator_b66a4p {
    void** vtable;
    void Free(UnkRefNode_b66a4p* node) {
        reinterpret_cast<void (*)(UnkNodeAllocator_b66a4p*, UnkRefNode_b66a4p*)>(vtable[0x48 / 8])(this, node);
    }
};

struct UnkPoisonFlyReset_b66a4 {
    u8 pad_0x0[0x38];
    u32 count;  // 0x38
    u8 pad_0x3c[4];
    UnkRefNode_b66a4p* head;  // 0x40
    UnkRefNode_b66a4p* tail;  // 0x48
    UnkNodeAllocator_b66a4p* allocator;  // 0x50
    u8 pad_0x58[0xa0 - 0x58];
    UnkRefCounted_b66a4p* ref;  // 0xa0
    u8 pad_0xa8[0xb4 - 0xa8];
    u32 field_0xb4;
};

static inline __attribute__((always_inline)) void Release_b66a4p(UnkRefCounted_b66a4p* p) {
    if (--p->ref_count == 0)
        p->vf_0x10();
}

bool fn_7100639ec0(UnkPoisonFlyReset_b66a4* self) {
    for (UnkRefNode_b66a4p* n = self->head; n;) {
        UnkRefNode_b66a4p* cur = n;
        n = n->next;
        if (cur->obj)
            Release_b66a4p(cur->obj);
        self->allocator->Free(cur);
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    if (self->ref) {
        Release_b66a4p(self->ref);
        self->ref = nullptr;
    }
    self->field_0xb4 = *reinterpret_cast<const u32*>(reinterpret_cast<const char*>(&g_unk_71726fa0f8) + 0x4c);
    return true;
}
