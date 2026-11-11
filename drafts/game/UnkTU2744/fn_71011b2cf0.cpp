#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkRefObj_b0209 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkRef_b0209 {
    UnkRefObj_b0209* ptr;
    ~UnkRef_b0209() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkRefNode_b0209 {
    UnkRef_b0209 ref;
    UnkRefNode_b0209* next;
};

struct UnkRefList_b0209 {
    u32 count;
    UnkRefNode_b0209* head;
    UnkRefNode_b0209* tail;
    u8 unk_0x18[8];
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void FreeNodes() {
        for (UnkRefNode_b0209* n = head; n;) {
            UnkRefNode_b0209* next = n->next;
            n->ref.~UnkRef_b0209();
            allocator->vf_0x48(n);
            n = next;
        }
    }
    __attribute__((always_inline)) ~UnkRefList_b0209() {
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

extern void* g_unk_7101cb58c8[];
extern void* g_unk_7101cb58e8[];

struct UnkGuiRefOwner_b0209 {
    void** vtable;
    UnkRefList_b0209 list;
    __attribute__((always_inline)) ~UnkGuiRefOwner_b0209() { vtable = &g_unk_7101cb58e8[2]; }
};

struct UnkGuiPoolOwner_b0209 : UnkGuiRefOwner_b0209 {
    u64 field_0x30;
    base::UnkBlockPool pool;  // 0x38

    __attribute__((always_inline)) ~UnkGuiPoolOwner_b0209() {
        vtable = &g_unk_7101cb58c8[2];
        pool.field_0x3c = true;
        list.FreeNodes();
        list.count = 0;
        list.allocator = nullptr;
        list.head = nullptr;
        list.tail = nullptr;
    }
};
static_assert(__builtin_offsetof(UnkGuiPoolOwner_b0209, pool) == 0x38, "layout");

void fn_71011b2cf0(UnkGuiPoolOwner_b0209* self) {
    self->~UnkGuiPoolOwner_b0209();
}
