// behaviortree::CEmmyPatrolBaseAction unit (tools/tu_map.py, the name from reflection): clears the flag at
// +0x11c and the action's node list (+0x38: count, head, tail, allocator), resets a few fields, empties
// the list at +0x14728 of the owner's AI component when it is a CEmmyAIComponent, then calls
// fn_710051564c(self, 1). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/behaviortree/CEmmyPatrolBaseAction.h"
#include "types.h"

struct UnkNodeAllocator_b55 {
    void** vtable;
    void Free(void* node) { reinterpret_cast<void (*)(UnkNodeAllocator_b55*, void*)>(vtable[9])(this, node); }
};

template <class Node>
struct UnkNodeList_b55 {
    u32 count;
    u8 pad_0x4[4];
    Node* head;
    Node* tail;
    UnkNodeAllocator_b55* allocator;

    // `ends` is &head, passed in so a caller can pin its address.
    __attribute__((always_inline)) void Clear(Node** ends) {
        for (Node* node = ends[0]; node;) {
            Node* next = node->next;
            allocator->Free(node);
            node = next;
        }
        count = 0;
        ends[0] = nullptr;
        ends[1] = nullptr;
    }
    __attribute__((always_inline)) void Clear() { Clear(&head); }
};

struct UnkPatrolNode_b55 {
    u8 pad_0x0[8];
    UnkPatrolNode_b55* next;
};

struct UnkEmmyNode_b55 {
    u8 pad_0x0[0x20];
    UnkEmmyNode_b55* next;
};

struct UnkEmmyNodeSet_b55 {
    u8 pad_0x0[0x58];
    UnkNodeList_b55<UnkEmmyNode_b55> list;
};

struct UnkEmmyAI_b55 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x146c8];
    UnkEmmyNodeSet_b55 nodes;  // 0x146d0
};

struct UnkPatrolOwner_b55 {
    u8 pad_0x0[0x240];
    UnkEmmyAI_b55* ai;
};

struct UnkPatrolBase_b55 {
    u8 pad_0x0[0x18];
    UnkPatrolOwner_b55* owner;
    u8 pad_0x20[0xc];
    s32 field_0x2c;
    u8 pad_0x30[8];
    UnkNodeList_b55<UnkPatrolNode_b55> list;  // 0x38
    u8 pad_0x58[0x50];
    u32 field_0xa8;
    u8 pad_0xac[0x24];
    u32 field_0xd0;
    u8 pad_0xd4[0x48];
    bool field_0x11c;
};

void fn_7100515cc8(UnkPatrolBase_b55* self) {
    self->field_0x11c = false;
    self->list.Clear();
    self->field_0x2c = -1;
    self->field_0xa8 = 0;
    self->field_0xd0 = 0;
    UnkEmmyAI_b55* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934())) {
        UnkEmmyNodeSet_b55* nodes = &ai->nodes;
        UnkEmmyNode_b55** ends = &ai->nodes.list.head;
        asm("" : "+r"(nodes), "+r"(ends));
        nodes->list.Clear(ends);
    }
    fn_710051564c(self, true);
}
