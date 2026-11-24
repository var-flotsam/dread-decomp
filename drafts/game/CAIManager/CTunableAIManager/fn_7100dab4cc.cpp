// CTunableAIManager's unit: removes the list nodes (count, head, tail and allocator at +0x2530..+0x2548) whose
// refcounted holder has no target or targets the object given: the node's item is released to the render
// manager, the holder let go and the node freed. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkTarget_ldab4cc;

struct UnkHolder_ldab4cc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
    u8 pad_0xc[4];
    UnkTarget_ldab4cc* target;  // 0x10
};

struct UnkHolderRef_ldab4cc {
    UnkHolder_ldab4cc* ptr;

    ~UnkHolderRef_ldab4cc() {
        if (UnkHolder_ldab4cc* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkNode_ldab4cc {
    UnkItemBase_71011a91c4* item;  // 0x0
    UnkHolderRef_ldab4cc holder;   // 0x8
    UnkNode_ldab4cc* next;         // 0x10
};

struct UnkMgr_ldab4cc {
    u8 pad_0x0[0x2530];
    u32 count;  // 0x2530
    u8 pad_0x2534[4];
    UnkNode_ldab4cc* head;  // 0x2538
    UnkNode_ldab4cc* tail;  // 0x2540
    base::IAllocator* allocator;  // 0x2548

    UnkNode_ldab4cc* Unlink(UnkNode_ldab4cc* node) {
        UnkNode_ldab4cc* prev = nullptr;
        UnkNode_ldab4cc* cur = head;
        UnkNode_ldab4cc* next;
        UnkNode_ldab4cc* last = nullptr;
        for (;;) {
            if (!cur)
                return nullptr;
            prev = last;
            next = cur->next;
            if (cur == node)
                break;
            last = cur;
            cur = next;
        }
        if (prev) {
            prev->next = next;
            if (tail == node)
                tail = prev;
        } else {
            head = next;
        }
        if (--count == 0) {
            head = nullptr;
            tail = nullptr;
        }
        return next;
    }
};

void fn_7100dab4cc(UnkMgr_ldab4cc* self, UnkTarget_ldab4cc* target) {
    if (!target)
        return;
    UnkNode_ldab4cc* node = self->head;
    while (node) {
        UnkHolder_ldab4cc* holder = node->holder.ptr;
        if (holder && holder->target && holder->target != target) {
            node = node->next;
            continue;
        }
        UnkNode_ldab4cc* next = self->Unlink(node);
        if (node->item) {
            fn_71002199d8(g_unk_7101d47300, node->item, 0);
            node->item = nullptr;
        }
        node->holder.~UnkHolderRef_ldab4cc();
        self->allocator->vf_0x48(node);
        node = next;
    }
}
