// Unit 852, fn_710059c970: adds `actor` to a list of weak references unless the list already refers
// to it (the list's nodes come from its allocator and are appended at the tail).
// Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "game_s2/UnkWeakHandle.h"

struct UnkWeakRefNode_59c970 {
    UnkWeakHandle* ref;
    UnkWeakRefNode_59c970* next;
};

using UnkWeakRefList_59c970 = base::UnkLinkedList<UnkWeakRefNode_59c970>;

struct UnkWeakRefOwner_59c970 {
    u8 pad_0x0[0x80];
    UnkWeakRefList_59c970 refs;
};

// A counted reference to a weak handle, held for the length of a scope.
struct UnkHeldWeakRef_59c970 {
    UnkWeakHandle* handle;

    explicit UnkHeldWeakRef_59c970(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHeldWeakRef_59c970() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

static inline s32 IndexOf(UnkWeakRefList_59c970* list, UnkWeakTarget* actor) {
    UnkHeldWeakRef_59c970 ref(GetWeakHandle(actor));
    s32 index = 0;
    for (UnkWeakRefNode_59c970* node = list->head; node; node = node->next, ++index) {
        if (TargetOf(node->ref) == TargetOf(ref.handle))
            return index;
    }
    return -1;
}

static inline void Append(UnkWeakRefList_59c970* list, UnkWeakTarget* actor) {
    UnkHeldWeakRef_59c970 ref(GetWeakHandle(actor));
    auto* node = static_cast<UnkWeakRefNode_59c970*>(
        list->allocator->vf_0x40(sizeof(UnkWeakRefNode_59c970), 8, list->flags));
    if (node) {
        node->ref = nullptr;
        AssignRef(&node->ref, ref.handle);
        node->next = nullptr;
        list->PushBack(node);
    }
}

void fn_710059c970(void* owner_, void* actor_) {
    UnkWeakRefList_59c970* list = &static_cast<UnkWeakRefOwner_59c970*>(owner_)->refs;
    UnkWeakTarget* actor = static_cast<UnkWeakTarget*>(actor_);
    if (IndexOf(list, actor) >= 0)
        return;
    Append(list, actor);
}
