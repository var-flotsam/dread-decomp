// Unit 2730 (tools/tu_map.py): moves the pending weak references of the cutscene manager into its tracked list
// (each target once, by a new weak handle of its own), then empties the pending list. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkLinkedList.h"
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100decc8c(UnkGameManager* self);

struct UnkHandleNode_l19818c {
    UnkWeakHandle* handle;
    UnkHandleNode_l19818c* next;

    explicit UnkHandleNode_l19818c(UnkWeakHandle* h) : handle(nullptr) {
        AssignRef(&handle, h);
        next = nullptr;
    }
};

// A counted reference held while it is in use.
struct UnkHandleRef_l19818c {
    UnkWeakHandle* p;

    explicit UnkHandleRef_l19818c(UnkWeakHandle* handle) : p(handle) {
        if (p)
            ++p->ref_count;
    }
    ~UnkHandleRef_l19818c() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// A singly linked list of weak handles whose nodes come from its allocator.
struct UnkHandleList_l19818c {
    void* vtable;
    base::UnkLinkedList<UnkHandleNode_l19818c> list;  // 0x8

    bool Contains(UnkWeakTarget* target) const {
        for (UnkHandleNode_l19818c* node = list.head; node; node = node->next)
            if (node->handle && node->handle->target == target)
                return true;
        return false;
    }

    void PushBack(const UnkHandleRef_l19818c& handle) {
        void* mem = list.allocator->vf_0x40(sizeof(UnkHandleNode_l19818c), 8, list.flags);
        if (mem)
            list.PushBack(new (mem) UnkHandleNode_l19818c(handle.p));
    }

    void Clear() {
        for (UnkHandleNode_l19818c* node = list.head; node;) {
            UnkHandleNode_l19818c* next = node->next;
            if (UnkWeakHandle* handle = node->handle) {
                if (--handle->ref_count == 0)
                    handle->vf_0x10();
            }
            list.allocator->vf_0x48(node);
            node = next;
        }
        list.count = 0;
        list.head = nullptr;
        list.tail = nullptr;
    }
};
static_assert(sizeof(UnkHandleList_l19818c) == 0x30, "layout");

struct UnkCutsceneManager_l19818c {
    u8 pad_0x0[0x60];
    UnkHandleList_l19818c tracked;  // 0x60
    UnkHandleList_l19818c pending;  // 0x90
};

void fn_710119818c(UnkCutsceneManager_l19818c* self) {
    fn_7100decc8c(g_unk_7101d47310);
    for (UnkHandleNode_l19818c* node = self->pending.list.head; node; node = node->next) {
        UnkWeakHandle* pending = node->handle;
        if (!pending || !pending->target)
            continue;
        UnkWeakTarget* target = pending->target;
        if (self->tracked.Contains(target))
            continue;
        UnkHandleRef_l19818c handle(fn_7100d78378(target));
        self->tracked.PushBack(handle);
    }
    self->pending.Clear();
}
