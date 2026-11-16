// Unit 2730 (tools/tu_map.py): moves the pending weak references of the cutscene manager into its tracked list
// (each target once, by a new weak handle of its own), then empties the pending list. Names and layouts are
// placeholders.
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
    u32 count;  // 0x8
    UnkHandleNode_l19818c* head;
    UnkHandleNode_l19818c* tail;
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;

    bool Contains(UnkWeakTarget* target) const {
        for (UnkHandleNode_l19818c* node = head; node; node = node->next)
            if (node->handle && node->handle->target == target)
                return true;
        return false;
    }

    void PushBack(const UnkHandleRef_l19818c& handle) {
        auto* node = static_cast<UnkHandleNode_l19818c*>(allocator->vf_0x40(sizeof(UnkHandleNode_l19818c), 8, flags));
        if (node) {
            new (node) UnkHandleNode_l19818c(handle.p);
            if (count == 0) {
                head = node;
                tail = node;
                count = 1;
            } else {
                tail->next = node;
                node->next = nullptr;
                tail = node;
                ++count;
            }
        }
    }

    void Clear() {
        for (UnkHandleNode_l19818c* node = head; node;) {
            UnkHandleNode_l19818c* next = node->next;
            if (UnkWeakHandle* handle = node->handle) {
                if (--handle->ref_count == 0)
                    handle->vf_0x10();
            }
            allocator->vf_0x48(node);
            node = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
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
    for (UnkHandleNode_l19818c* node = self->pending.head; node; node = node->next) {
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
