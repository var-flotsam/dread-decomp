// Unit 2568 (tools/tu_map.py): the destructors of an object (vtable 0x7101ca9f90) holding a singly linked list
// of nodes (count +8, head +0x10, tail +0x18, allocator +0x20); each node's counted reference at +8 is dropped
// (fn_710001737c on the last one) before the allocator frees the node. fn_7100ec2f58 is the plain form,
// fn_7100ec2fe4 the deleting one. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101ca9f90[];

struct UnkRefNode_A6_b72 {
    void* field_0x0;
    UnkCountedResource* ref;
    UnkRefNode_A6_b72* next;
};

struct UnkRefList_A6_b72 {
    void** vtable;
    u32 count;
    UnkRefNode_A6_b72* head;
    UnkRefNode_A6_b72* tail;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<void**>(reinterpret_cast<char*>(g_unk_7101ca9f90) + 0x10);
        for (UnkRefNode_A6_b72* node = head; node;) {
            UnkRefNode_A6_b72* next = node->next;
            if (UnkCountedResource* r = node->ref) {
                if (--r->ref_count == 0)
                    fn_710001737c(r);
            }
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

void fn_7100ec2f58(UnkRefList_A6_b72* self) {
    self->Destroy();
}

void fn_7100ec2fe4(UnkRefList_A6_b72* self) {
    self->Destroy();
    __builtin_operator_delete(self);
}
