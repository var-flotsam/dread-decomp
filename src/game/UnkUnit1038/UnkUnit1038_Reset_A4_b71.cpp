// Unit 1038 (tools/tu_map.py, no class name): after fn_71006f345c, empties the two lists of refcounted
// pointers at +0x260 and +0x2d8 (count, head, tail, allocator), releasing each pointer and freeing each
// node; returns true. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "types.h"

void fn_71006f345c(void* self);

struct UnkU1038Node_A4_b71 {
    base::UnkRefPtrMember ref;
    UnkU1038Node_A4_b71* next;
};

struct UnkU1038List_A4_b71 {
    u8 pad_0x0[8];
    u32 count;
    UnkU1038Node_A4_b71* head;
    UnkU1038Node_A4_b71* tail;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Clear() {
        for (UnkU1038Node_A4_b71* n = head; n;) {
            UnkU1038Node_A4_b71* cur = n;
            n = n->next;
            cur->ref.~UnkRefPtrMember();
            allocator->vf_0x48(cur);
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkU1038Self_A4_b71 {
    u8 pad_0x0[0x260];
    UnkU1038List_A4_b71 a;  // 0x260
    u8 pad_0x288[0x2d8 - 0x288];
    UnkU1038List_A4_b71 b;  // 0x2d8
};

bool fn_71006a1e04(UnkU1038Self_A4_b71* self) {
    fn_71006f345c(self);
    self->a.Clear();
    self->b.Clear();
    return true;
}
