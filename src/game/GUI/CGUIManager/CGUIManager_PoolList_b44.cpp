// CGUIManager's unit (gui/scripts/guimanager.lua, tools/tu_map.py): a pool-backed list's destructor and
// its deleting destructor, copies of fn_71010f9660 and fn_7101115cb4 (CAIComponent_PoolList_b44.cpp) with
// nodes linked at +0x8 and their own vtables. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

extern char g_unk_7101cb5888[];
extern char g_unk_7101cb58a8[];

struct UnkWidePoolListBase_b44a3g {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    u64 field_0x20;
    base::UnkAllocator* allocator;
    u32 flags;

    ~UnkWidePoolListBase_b44a3g() {
        vtable = g_unk_7101cb58a8 + 0x10;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    void FreeNodes() {
        for (char* n = head; n;) {
            char* next = *reinterpret_cast<char**>(n + 0x8);
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkWidePoolList_b44a3g : UnkWidePoolListBase_b44a3g {
    base::UnkBlockPool pool;  // 0x38

    ~UnkWidePoolList_b44a3g() {
        vtable = g_unk_7101cb5888 + 0x10;
        pool.field_0x3c = true;
        FreeNodes();
        count = 0;
        __builtin_memset(&head, 0, 0x10);
        allocator = nullptr;
    }
};
static_assert(sizeof(UnkWidePoolList_b44a3g) == 0x78, "UnkWidePoolList_b44a3g placeholder layout");

__attribute__((flatten)) void fn_71011b2bf8(UnkWidePoolList_b44a3g* self) {
    self->~UnkWidePoolList_b44a3g();
}

__attribute__((flatten)) void fn_71011bb98c(UnkWidePoolList_b44a3g* self) {
    self->~UnkWidePoolList_b44a3g();
    __builtin_operator_delete(self);
}
