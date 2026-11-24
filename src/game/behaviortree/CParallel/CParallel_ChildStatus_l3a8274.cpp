// behaviortree::CParallel's unit: a child reports success (5) or failure (6). After the base node's own
// handling (fn_7100394bd8) the child leaves the running list (+0x70) and is counted (+0xd8, +0xdc); once
// the policy for that result (+0x58, +0x5c: one child or all of them) is met, the running children the tree
// drops (fn_71003aad48) are removed and the result goes to the tree (fn_71003aac0c). Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "game/behaviortree/CBehaviorTree.h"
#include "types.h"

struct UnkSelectorNode_A2_b68;
struct UnkStruct_71003aad48;
struct UnkNode_7100394bd8;
void fn_7100394bd8(UnkNode_7100394bd8* self, UnkSelectorNode_A2_b68* child, s32 status);

struct UnkParallelEntry_l3a8274 {
    UnkSelectorNode_A2_b68* node;
    UnkParallelEntry_l3a8274* next;
};

struct UnkParallel_l3a8274 {
    u8 pad_0x0[0x10];
    void* tree;  // 0x10
    u8 pad_0x18[0x20];
    s32 child_count;  // 0x38: the children vector's count
    u8 pad_0x3c[0x1c];
    s32 success_policy;  // 0x58: 0 one child, else all of them
    s32 failure_policy;  // 0x5c
    s32 state;           // 0x60
    u8 pad_0x64[0xc];
    u32 running_count;               // 0x70
    UnkParallelEntry_l3a8274* head;  // 0x78
    UnkParallelEntry_l3a8274* tail;  // 0x80
    base::UnkAllocator* allocator;   // 0x88
    u32 flags;                       // 0x90
    u8 pad_0x94[0x44];
    s32 succeeded;  // 0xd8
    s32 failed;     // 0xdc
    u32 pending;    // 0xe0

    UnkParallelEntry_l3a8274* Find(UnkSelectorNode_A2_b68* node) {
        for (UnkParallelEntry_l3a8274* e = head; e; e = e->next) {
            if (e->node == node)
                return e;
        }
        return nullptr;
    }

    // Returns the entry after the one taken out.
    UnkParallelEntry_l3a8274* Erase(UnkParallelEntry_l3a8274* entry) {
        UnkParallelEntry_l3a8274* prev = nullptr;
        UnkParallelEntry_l3a8274* it = head;
        while (it) {
            UnkParallelEntry_l3a8274* cur = it;
            UnkParallelEntry_l3a8274* next = it->next;
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
};
static_assert(__builtin_offsetof(UnkParallel_l3a8274, pending) == 0xe0, "UnkParallel_l3a8274 layout");

// Drops the running children the tree lets go; those it keeps are counted as pending. With none left
// running the tree gets `Status` now, otherwise it becomes the node's state.
template <s32 Status>
static void Finish(UnkParallel_l3a8274* self) {
    if (UnkParallelEntry_l3a8274* e = self->head) {
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
        self->Notify(Status);
        return;
    }
    self->state = Status;
}

void CParallel_RemoveChildAndApplyResultPolicy(UnkParallel_l3a8274* self, UnkSelectorNode_A2_b68* child, s32 status) {
    fn_7100394bd8(reinterpret_cast<UnkNode_7100394bd8*>(self), child, status);
    UnkParallelEntry_l3a8274* entry = self->Find(child);
    if (!entry)
        return;
    self->Erase(entry);
    switch (status) {
    case 5: {
        s32 succeeded = ++self->succeeded;
        if (self->success_policy == 0) {
            if (succeeded >= 1)
                Finish<5>(self);
        } else if (succeeded >= self->child_count) {
            self->Notify(5);
        }
        break;
    }
    case 6: {
        s32 failed = ++self->failed;
        if (self->failure_policy == 0) {
            if (failed >= 1)
                Finish<6>(self);
        } else if (failed >= self->child_count) {
            self->Notify(6);
        }
        break;
    }
    }
}
