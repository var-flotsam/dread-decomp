// Unit 852, fn_710059d000: adds `actor` to the record's list of weak references unless the list
// already refers to it, and makes it the record's current target when that refers to nothing.
// Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkWeakRefNode_59d000 {
    UnkWeakHandle* ref;
    UnkWeakRefNode_59d000* next;
};

using UnkWeakRefList_59d000 = base::UnkLinkedList<UnkWeakRefNode_59d000>;

struct UnkTargetRecord_59d000 {
    u8 pad_0x0[0x10];
    UnkWeakRefList_59d000 targets;
    u8 pad_0x38[0xe8 - 0x38];
    UnkWeakHandle* current;
};
static_assert(__builtin_offsetof(UnkTargetRecord_59d000, current) == 0xe8, "layout");

// A counted reference to a weak handle, held for the length of a scope.
struct UnkHeldWeakRef_59d000 {
    UnkWeakHandle* handle;

    explicit UnkHeldWeakRef_59d000(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHeldWeakRef_59d000() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

static inline s32 IndexOf(UnkWeakRefList_59d000* list, UnkWeakTarget* actor) {
    UnkHeldWeakRef_59d000 ref(GetWeakHandle(actor));
    s32 index = 0;
    for (UnkWeakRefNode_59d000* node = list->head; node; node = node->next, ++index) {
        if (TargetOf(node->ref) == TargetOf(ref.handle))
            return index;
    }
    return -1;
}

static inline void Append(UnkWeakRefList_59d000* list, UnkWeakTarget* actor) {
    UnkHeldWeakRef_59d000 ref(GetWeakHandle(actor));
    auto* node = static_cast<UnkWeakRefNode_59d000*>(
        list->allocator->vf_0x40(sizeof(UnkWeakRefNode_59d000), 8, list->flags));
    if (node) {
        node->ref = nullptr;
        AssignRef(&node->ref, ref.handle);
        node->next = nullptr;
        list->PushBack(node);
    }
}

void fn_710059d000(UnkTargetRecord_59d000* self, UnkWeakTarget* actor) {
    if (IndexOf(&self->targets, actor) >= 0)
        return;
    Append(&self->targets, actor);
    if (!TargetOf(self->current))
        AssignRef(&self->current, GetWeakHandle(actor));
}
