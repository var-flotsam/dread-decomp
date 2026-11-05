// behaviortree::CEmmyPatrolBaseAction unit (tools/tu_map.py unit 803): clears the flag at +0x11c; when
// the owner's AI component is a CEmmyAIComponent with a patrol path (+0x14708), counts one more step
// (+0xf0), empties the component's node list at +0x14728, and while the count is within the path's limit
// (fn_71006fa894) moves on (fn_710051564c(self, 1)). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

// As src/game/behaviortree/CEmmyPatrolBaseAction/CEmmyPatrolBaseAction_Start_b55.cpp declares it.
struct UnkPatrolBase_b55;
void fn_710051564c(UnkPatrolBase_b55* self, bool flag);

u32 fn_71006fa894(void* path);

struct UnkNodeAllocator_A3_b67 {
    void** vtable;
    void Free(void* node) { reinterpret_cast<void (*)(UnkNodeAllocator_A3_b67*, void*)>(vtable[9])(this, node); }
};

struct UnkEmmyNode_A3_b67 {
    u8 pad_0x0[0x20];
    UnkEmmyNode_A3_b67* next;
};

struct UnkEmmyNodeSet_A3_b67 {
    u8 pad_0x0[0x38];
    void* path;  // 0x38
    u8 pad_0x40[0x18];
    u32 count;  // 0x58
    u8 pad_0x5c[4];
    UnkEmmyNode_A3_b67* head;  // 0x60
    UnkEmmyNode_A3_b67* tail;  // 0x68
    UnkNodeAllocator_A3_b67* allocator;  // 0x70
};

struct UnkEmmyAI_A3_b67 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x146c8];
    UnkEmmyNodeSet_A3_b67 nodes;  // 0x146d0
};

struct UnkPatrolOwner_A3_b67 {
    u8 pad_0x0[0x240];
    UnkEmmyAI_A3_b67* ai;
};

struct UnkPatrolAction_A3_b67 {
    u8 pad_0x0[0x18];
    UnkPatrolOwner_A3_b67* owner;
    u8 pad_0x20[0xd0];
    u32 steps;  // 0xf0
    u8 pad_0xf4[0x28];
    bool field_0x11c;
};

void fn_7100515ed8(UnkPatrolAction_A3_b67* self) {
    self->field_0x11c = false;
    UnkEmmyAI_A3_b67* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()) && ai->nodes.path) {
        UnkEmmyNodeSet_A3_b67* set = &ai->nodes;
        u32 steps = ++self->steps;
        asm("" : "+r"(set));
        u32 limit = fn_71006fa894(set->path);
        for (UnkEmmyNode_A3_b67* node = set->head; node;) {
            UnkEmmyNode_A3_b67* next = node->next;
            set->allocator->Free(node);
            node = next;
        }
        set->count = 0;
        set->head = nullptr;
        set->tail = nullptr;
        if (steps <= limit)
            fn_710051564c(reinterpret_cast<UnkPatrolBase_b55*>(self), true);
    }
}
