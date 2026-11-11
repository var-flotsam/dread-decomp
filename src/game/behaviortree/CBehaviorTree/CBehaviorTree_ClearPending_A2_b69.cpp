// behaviortree::CBehaviorTree's unit (the name from reflection): clears the list of pending entries (the list of
// src/game/behaviortree/CBehaviorTree/fn_71003aad48.cpp): each entry's node is told fn_7100394b30(node, 0, 7),
// then every entry is freed through the allocator (vtable slot 0x48) and the list is emptied.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkBehaviorNode_A2_b69;
s32 fn_7100394b30(UnkBehaviorNode_A2_b69* node, f32 value, s32 state);

struct UnkPendingEntry_A2_b69 {
    void* vtable;
    u32 state;
    UnkBehaviorNode_A2_b69* key;     // +0x10
    UnkPendingEntry_A2_b69* next;    // +0x18
};

struct UnkPendingList_A2_b69 {
    u8 pad_0x0[0x20];
    u32 count;                       // +0x20
    UnkPendingEntry_A2_b69* head;    // +0x28
    UnkPendingEntry_A2_b69* tail;    // +0x30
    base::UnkAllocator* allocator;   // +0x38
};

void fn_71003aaad0(UnkPendingList_A2_b69* self) {
    if (self->head) {
        for (UnkPendingEntry_A2_b69* e = self->head; e; e = e->next)
            if (e->key)
                fn_7100394b30(e->key, 0.0f, 7);
        for (UnkPendingEntry_A2_b69* e = self->head; e;) {
            UnkPendingEntry_A2_b69* next = e->next;
            self->allocator->vf_0x48(e);
            e = next;
        }
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
