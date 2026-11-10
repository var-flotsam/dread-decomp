// behaviortree::CParallel's unit (tools/tu_map.py, the name from reflection): starting the children.
// It gives 5 while anything is still counted, pending or running; otherwise every child, from the
// last, is started on the tree (+0x10, fn_71003aab64) and those that started are recorded in the
// running list (+0x70: count, head, tail; entries from the allocator at +0x88 with the flags at
// +0x90). The result is 4 when any started, else 6. Also the destructor (fn_71003a7db4): the vtable,
// then the running list at +0x68 (a pooled node list, base/UnkNodeOwner.h: next link at +0x8, nothing
// to destroy in a node), then the node's base destructor (fn_71003961e8). Names and layouts are
// placeholders.
#include "UnkClone.h"
#include "base/UnkNodeOwner.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkClone_fn_710039657c;
using UnkParallelChild_ovb0116 = UnkCloneElem<UnkClone_fn_710039657c, 8>;
extern template UnkParallelChild_ovb0116& base::UnkVector<UnkParallelChild_ovb0116>::GrowAt(u32);

struct UnkSelectorTree_A2_b68;
struct UnkSelectorNode_A2_b68;
void fn_71003aab64(UnkSelectorTree_A2_b68* tree, UnkSelectorNode_A2_b68* child);

struct UnkParallelEntry_ovb0116 {
    UnkSelectorNode_A2_b68* node;
    UnkParallelEntry_ovb0116* next;
};

struct UnkParallel_ovb0116 {
    u8 pad_0x0[0x10];
    UnkSelectorTree_A2_b68* tree;  // 0x10
    u8 pad_0x18[0x18];
    base::UnkVector<UnkParallelChild_ovb0116> children;  // 0x30
    u8 pad_0x58[0x8];
    s32 state;  // 0x60
    u8 pad_0x64[0xc];
    u32 running_count;                  // 0x70
    UnkParallelEntry_ovb0116* head;     // 0x78
    UnkParallelEntry_ovb0116* tail;     // 0x80
    base::UnkAllocator* allocator;      // 0x88
    u32 flags;                          // 0x90
    u8 pad_0x94[0x44];
    u32 field_0xd8;
    u32 field_0xdc;
    u32 pending;  // 0xe0: children the tree didn't drop yet
};
static_assert(__builtin_offsetof(UnkParallel_ovb0116, field_0xd8) == 0xd8, "UnkParallel_ovb0116 layout");

// The tree's start call reports whether the child started (the shared declaration has it void).
static inline bool Start_ovb0116(UnkSelectorTree_A2_b68* tree, UnkSelectorNode_A2_b68* child) {
    return reinterpret_cast<bool (*)(UnkSelectorTree_A2_b68*, UnkSelectorNode_A2_b68*)>(&fn_71003aab64)(tree, child);
}

s32 fn_71003a8044(UnkParallel_ovb0116* self) {
    if (self->field_0xd8 || self->field_0xdc || self->pending || self->running_count)
        return 5;
    for (u32 i = self->children.count - 1; (s32)i >= 0; --i) {
        UnkSelectorNode_A2_b68* child =
            *reinterpret_cast<UnkSelectorNode_A2_b68**>(self->children.GrowAt(i).data);
        if (!child)
            continue;
        if (!Start_ovb0116(self->tree, child))
            continue;
        UnkParallelEntry_ovb0116* entry = static_cast<UnkParallelEntry_ovb0116*>(
            self->allocator->vf_0x40(sizeof(UnkParallelEntry_ovb0116), 8, self->flags));
        if (!entry)
            continue;
        entry->node = child;
        entry->next = nullptr;
        if (self->running_count == 0) {
            self->head = entry;
            self->tail = entry;
            self->running_count = 1;
        } else {
            // Copied bytewise: the original reloads the count after this store.
            __builtin_memcpy(&self->tail->next, &entry, sizeof(entry));
            entry->next = nullptr;
            self->tail = entry;
            ++self->running_count;
        }
    }
    return self->running_count == 0 ? 6 : 4;
}

extern void* g_unk_7101a29148[];
extern void* g_unk_7101a291f8[];  // the running list's derived part's vtable
extern void* g_unk_7101a29218[];  // its base part's
struct UnkBehaviorNode_ovb0116;
void fn_71003961e8(UnkBehaviorNode_ovb0116* self);

template <u32 Next>
__attribute__((always_inline)) inline void UnkKeepNode_ovb0116(UnkListNode<Next>*) {}

using UnkParallelList_ovb0116 = UnkNodeOwner<UnkListNode<8>, UnkKeepNode_ovb0116<8>>;

struct UnkParallelDtor_ovb0116 {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkParallelList_ovb0116 running;  // 0x68
};
static_assert(sizeof(UnkParallelDtor_ovb0116) == 0xd8, "UnkParallelDtor_ovb0116 layout");

void fn_71003a7db4(UnkParallelDtor_ovb0116* self) {
    self->vtable = g_unk_7101a29148 + 2;
    self->running.Destroy(g_unk_7101a291f8, g_unk_7101a29218);
    fn_71003961e8(reinterpret_cast<UnkBehaviorNode_ovb0116*>(self));
}
