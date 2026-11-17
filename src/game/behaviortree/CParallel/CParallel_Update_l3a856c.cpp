// behaviortree::CParallel's unit: a child finished. It is taken out of the running list (+0x70); while
// children the tree has yet to drop are counted (+0xe0) the last one reports the node's state, otherwise
// every running child the tree drops (fn_71003aad48) is removed and the node reports 3 or 7 to the tree
// (fn_71003aac0c). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkSelectorNode_A2_b68;

struct UnkParallelEntry_l3a856c {
    UnkSelectorNode_A2_b68* node;
    UnkParallelEntry_l3a856c* next;
};

struct UnkParallel_l3a856c {
    u8 pad_0x0[0x10];
    void* tree;  // 0x10
    u8 pad_0x18[0x48];
    s32 state;  // 0x60
    u8 pad_0x64[0xc];
    u32 running_count;                 // 0x70
    UnkParallelEntry_l3a856c* head;    // 0x78
    UnkParallelEntry_l3a856c* tail;    // 0x80
    base::UnkAllocator* allocator;     // 0x88
    u32 flags;                         // 0x90
    u8 pad_0x94[0x4c];
    u32 pending;  // 0xe0

    UnkParallelEntry_l3a856c* Find(UnkSelectorNode_A2_b68* node) {
        for (UnkParallelEntry_l3a856c* e = head; e; e = e->next) {
            if (e->node == node)
                return e;
        }
        return nullptr;
    }

    // Returns the entry after the one taken out.
    UnkParallelEntry_l3a856c* Erase(UnkParallelEntry_l3a856c* entry) {
        UnkParallelEntry_l3a856c* prev = nullptr;
        UnkParallelEntry_l3a856c* it = head;
        while (it) {
            UnkParallelEntry_l3a856c* cur = it;
            UnkParallelEntry_l3a856c* next = it->next;
            it = next;
            if (cur != entry) {
                prev = cur;
                continue;
            }
            if (prev) {
                prev->next = next;
                if (tail == entry)
                    tail = prev;
            } else {
                head = next;
            }
            if (--running_count == 0) {
                head = nullptr;
                tail = nullptr;
            }
            allocator->vf_0x48(entry);
            return it;
        }
        return it;
    }

    void Notify(s32 status) {
        fn_71003aac0c(static_cast<UnkInverterTarget_b66a2*>(tree), reinterpret_cast<UnkInverter_b66a2*>(this),
                      status);
    }

    // Reports the state the node already holds.
    void NotifyState() {
        fn_71003aac0c(static_cast<UnkInverterTarget_b66a2*>(tree), reinterpret_cast<UnkInverter_b66a2*>(this),
                      state);
    }
};
static_assert(__builtin_offsetof(UnkParallel_l3a856c, pending) == 0xe0, "UnkParallel_l3a856c layout");

void fn_71003a856c(UnkParallel_l3a856c* self, UnkSelectorNode_A2_b68* child) {
    UnkParallelEntry_l3a856c* entry = self->Find(child);
    if (!entry)
        return;
    self->Erase(entry);
    if (self->pending) {
        if (--self->pending == 0)
            self->NotifyState();
        return;
    }
    if (self->running_count == 0) {
        self->Notify(3);
        return;
    }
    if (UnkParallelEntry_l3a856c* e = self->head) {
        do {
            if (fn_71003aad48(static_cast<UnkStruct_71003aad48*>(self->tree), e->node)) {
                e = self->Erase(e);
            } else {
                ++self->pending;
                e = e->next;
            }
        } while (e);
    }
    if (self->running_count == 0) {
        self->Notify(7);
        return;
    }
    self->state = 7;
}
