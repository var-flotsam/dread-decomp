// Destructors (complete and deleting) of an object (vtable unk_7101a20260) whose base has nothing
// to destroy, with a node list at 0x228 (vtable unk_7101a202c0, count at 0x230, head and tail at
// 0x238/0x240, allocator at 0x248; nodes link through +8): free the nodes and empty the list
// (fn_7100218a2c, fn_7100218b04). The deleting one frees through __builtin_operator_delete, so
// the stores emptying the list are dropped. fn_71010cf1e8 is the same for another object. Called
// only through the vtable.

#include "base/UnkVector.h"
#include "types.h"

extern char unk_7101a20260[];
extern char unk_7101a202c0[];

struct UnkNode_7100218a2c {
    u64 field_0x0;
    UnkNode_7100218a2c* next;
};

struct UnkNodeList_7100218a2c {
    char* vtable;
    u32 count;
    UnkNode_7100218a2c* head;
    UnkNode_7100218a2c* tail;
    base::UnkAllocator* allocator;

    ~UnkNodeList_7100218a2c() {
        vtable = unk_7101a202c0 + 0x10;
        for (UnkNode_7100218a2c* n = head; n;) {
            UnkNode_7100218a2c* next = n->next;
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkObject_7100218a2c {
    char* vtable;
    u8 unk_0x8[0x220];
    UnkNodeList_7100218a2c list;

    ~UnkObject_7100218a2c() { vtable = unk_7101a20260 + 0x10; }
};

void fn_7100218a2c(UnkObject_7100218a2c* self) { self->~UnkObject_7100218a2c(); }

void fn_7100218b04(UnkObject_7100218a2c* self) {
    self->~UnkObject_7100218a2c();
    __builtin_operator_delete(self);
}
