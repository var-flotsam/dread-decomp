// Two copies of one function, in the behaviortree::CEmmySearchingAction unit (fn_710051cb3c) and the
// CEmmyAIComponent unit (fn_7100560a78, its own node vtable): empties a node list (count, head,
// tail, allocator, allocation tag), picks one of the owner's start nodes at random (count at
// +0x12cc, pointers from +0x12d0) and pushes a copy of every node of the chain it starts (linked
// through +0x10) to the front of the list. The list follows the node-list shape of
// CEmmyPatrolBaseAction_Start_b55.cpp. Names and layouts are placeholders.
#include "game/UnkTU811.h"
#include "game/behaviortree/CEmmySearchingAction.h"
#include "types.h"

extern "C" int rand();

struct UnkPathAllocator_b56a3 {
    void** vtable;
    void* Alloc(u64 size, u64 align, u32 tag) {
        return reinterpret_cast<void* (*)(UnkPathAllocator_b56a3*, u64, u64, u32)>(vtable[8])(this, size, align, tag);
    }
    void Free(void* node) { reinterpret_cast<void (*)(UnkPathAllocator_b56a3*, void*)>(vtable[9])(this, node); }
};

struct UnkPathSource_b56a3 {
    void* vtable;
    void* field_0x8;
    UnkPathSource_b56a3* next;
    u32 field_0x18;
    u32 field_0x1c;
};

struct UnkPathNode_b56a3 {
    void* vtable;
    void* field_0x8;
    UnkPathSource_b56a3* field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    UnkPathNode_b56a3* next;
};

struct UnkPathList_b56a3 {
    u8 pad_0x0[0x10];
    union {
        u32 count;
    };
    u8 pad_0x14[4];
    UnkPathNode_b56a3* head;
    UnkPathNode_b56a3* tail;
    UnkPathAllocator_b56a3* allocator;
    u32 tag;
};

struct UnkStarts_b56a3 {
    u32 count;
    UnkPathSource_b56a3* data[1];
} __attribute__((packed));

struct UnkPathOwner_b56a3 {
    u8 pad_0x0[0x12cc];
    UnkStarts_b56a3 starts;
};

static inline __attribute__((always_inline)) void PickPath_b56a3(UnkPathOwner_b56a3* self, UnkPathList_b56a3* list,
                                                                  void* vtable) {
    UnkPathNode_b56a3** ends = &list->head;
    for (UnkPathNode_b56a3* node = ends[0]; node;) {
        UnkPathNode_b56a3* next = node->next;
        list->allocator->Free(node);
        node = next;
    }
    asm("" : "+r"(ends));
    list->count = 0;
    ends[0] = nullptr;
    ends[1] = nullptr;
    u32 r = rand();
    UnkStarts_b56a3* starts = &self->starts;
    for (UnkPathSource_b56a3* src = starts->data[r % starts->count]; src; src = src->next) {
        UnkPathNode_b56a3* node = static_cast<UnkPathNode_b56a3*>(list->allocator->Alloc(0x28, 8, list->tag));
        if (!node)
            continue;
        node->vtable = vtable;
        node->field_0x8 = src->field_0x8;
        node->field_0x10 = src->next;
        node->field_0x18 = src->field_0x18;
        node->field_0x1c = src->field_0x1c;
        node->next = nullptr;
        if (list->count == 0) {
            list->head = node;
            list->tail = node;
            list->count = 1;
        } else {
            node->next = list->head;
            list->head = node;
            list->count = list->count + 1;
        }
    }
}

void fn_710051cb3c(UnkPathOwner_b56a3* self, UnkPathList_b56a3* list) {
    PickPath_b56a3(self, list, reinterpret_cast<char*>(g_unk_7101a47fa8) + 0x10);
}
void fn_7100560a78(UnkPathOwner_b56a3* self, UnkPathList_b56a3* list) {
    PickPath_b56a3(self, list, reinterpret_cast<char*>(g_unk_7101a48c20) + 0x10);
}
