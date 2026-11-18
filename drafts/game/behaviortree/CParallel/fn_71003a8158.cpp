// behaviortree::CParallel's unit: the node's abort. With children still running (+0x70) and none pending
// yet (+0xe0), the running children the tree drops (fn_71003aad48) are removed and those it keeps are
// counted as pending; the node then waits (4, its state 7) or is done (7). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkSelectorNode_A2_b68;
struct UnkStruct_71003aad48;

struct UnkParallelEntry_l3a8158 {
    UnkSelectorNode_A2_b68* node;
    UnkParallelEntry_l3a8158* next;
};

struct UnkParallel_l3a8158 {
    u8 pad_0x0[0x10];
    void* tree;  // 0x10
    u8 pad_0x18[0x48];
    s32 state;  // 0x60
    u8 pad_0x64[0xc];
    u32 running_count;               // 0x70
    UnkParallelEntry_l3a8158* head;  // 0x78
    UnkParallelEntry_l3a8158* tail;  // 0x80
    base::UnkAllocator* allocator;   // 0x88
    u32 flags;                       // 0x90
    u8 pad_0x94[0x4c];
    u32 pending;  // 0xe0

    // Returns the entry after the one taken out.
    UnkParallelEntry_l3a8158* Erase(UnkParallelEntry_l3a8158* entry) {
        UnkParallelEntry_l3a8158* prev = nullptr;
        UnkParallelEntry_l3a8158* it = head;
        while (it) {
            UnkParallelEntry_l3a8158* cur = it;
            UnkParallelEntry_l3a8158* next = it->next;
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
};
static_assert(__builtin_offsetof(UnkParallel_l3a8158, pending) == 0xe0, "UnkParallel_l3a8158 layout");

s32 fn_71003a8158(UnkParallel_l3a8158* self) {
    if (self->running_count != 0) {
        if (self->pending != 0) {
            self->state = 3;
            return 4;
        }
        if (UnkParallelEntry_l3a8158* e = self->head) {
            do {
                if (fn_71003aad48(static_cast<UnkStruct_71003aad48*>(self->tree), e->node)) {
                    e = self->Erase(e);
                } else {
                    ++self->pending;
                    e = e->next;
                }
            } while (e);
        }
        if (self->running_count != 0) {
            self->state = 7;
            return 4;
        }
    }
    return 7;
}
