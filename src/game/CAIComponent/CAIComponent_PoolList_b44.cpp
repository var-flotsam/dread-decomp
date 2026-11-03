// CAIComponent's unit (tools/tu_map.py): a pool-backed list's destructor and its deleting destructor
// (shaped like fn_71003f9ee8 and fn_71003fab14 in src/clones/, but with a word at 0x20 before the
// allocator, nodes linked at +0x18, and their own vtables). Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

extern char g_unk_7101cb25b8[];
extern char g_unk_7101cb25d8[];

struct UnkWidePoolListBase_b44a3 {
    char* vtable;
    u32 count;
    char* head;
    char* tail;
    u64 field_0x20;
    base::UnkAllocator* allocator;
    u32 flags;

    ~UnkWidePoolListBase_b44a3() {
        vtable = g_unk_7101cb25d8 + 0x10;
        FreeNodes();
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
    void FreeNodes() {
        for (char* n = head; n;) {
            char* next = *reinterpret_cast<char**>(n + 0x18);
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

struct UnkWidePoolList_b44a3 : UnkWidePoolListBase_b44a3 {
    base::UnkBlockPool pool;  // 0x38

    ~UnkWidePoolList_b44a3() {
        vtable = g_unk_7101cb25b8 + 0x10;
        pool.field_0x3c = true;
        FreeNodes();
        count = 0;
        __builtin_memset(&head, 0, 0x10);
        allocator = nullptr;
    }
};
static_assert(sizeof(UnkWidePoolList_b44a3) == 0x78, "UnkWidePoolList_b44a3 placeholder layout");

__attribute__((flatten)) void fn_71010f9660(UnkWidePoolList_b44a3* self) {
    self->~UnkWidePoolList_b44a3();
}

__attribute__((flatten)) void fn_7101115cb4(UnkWidePoolList_b44a3* self) {
    self->~UnkWidePoolList_b44a3();
    __builtin_operator_delete(self);
}
