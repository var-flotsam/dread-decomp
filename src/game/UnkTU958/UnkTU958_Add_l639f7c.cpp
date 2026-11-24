// Unit 958: adds an actor that has an "AI" component to the record's list of weak references unless the list
// already refers to it, and makes the record's current one the actor whenever the list holds just one.
// Names and layouts are placeholders.
#include "base/UnkLinkedList.h"
#include "base/reflection/UnkReflStatics.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkRefl_fn_710063a360;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_710063a360>(void*);

struct UnkMemberA_b0025;

struct UnkWeakRefNode_639f7c {
    UnkWeakHandle* ref;
    UnkWeakRefNode_639f7c* next;
};

using UnkWeakRefList_639f7c = base::UnkLinkedList<UnkWeakRefNode_639f7c>;

struct UnkAIList_639f7c {
    u8 pad_0x0[0x38];
    UnkWeakRefList_639f7c actors;  // 0x38
    u8 pad_0x60[0xa0 - 0x60];
    UnkWeakHandle* current;  // 0xa0
};
static_assert(__builtin_offsetof(UnkAIList_639f7c, current) == 0xa0, "layout");

// A counted reference to a weak handle, held for the length of a scope.
struct UnkHeldWeakRef_639f7c {
    UnkWeakHandle* handle;

    explicit UnkHeldWeakRef_639f7c(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHeldWeakRef_639f7c() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

static inline UnkWeakTarget* TargetOf(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

static inline s32 IndexOf(UnkWeakRefList_639f7c* list, UnkWeakTarget* actor) {
    UnkHeldWeakRef_639f7c ref(GetWeakHandle(actor));
    s32 index = 0;
    for (UnkWeakRefNode_639f7c* node = list->head; node; node = node->next, ++index) {
        if (TargetOf(node->ref) == TargetOf(ref.handle))
            return index;
    }
    return -1;
}

static inline void Append(UnkWeakRefList_639f7c* list, UnkWeakTarget* actor) {
    UnkHeldWeakRef_639f7c ref(GetWeakHandle(actor));
    auto* node = static_cast<UnkWeakRefNode_639f7c*>(
        list->allocator->vf_0x40(sizeof(UnkWeakRefNode_639f7c), 8, list->flags));
    if (node) {
        node->ref = nullptr;
        AssignRef(&node->ref, ref.handle);
        node->next = nullptr;
        list->PushBack(node);
    }
}

void AddWeakRefOnceAndSetCurrentIfOnlyEntry(UnkMemberA_b0025* self_, void* item) {
    UnkAIList_639f7c* self = reinterpret_cast<UnkAIList_639f7c*>(self_);
    if (!base::reflection::refl_FindTyped<UnkRefl_fn_710063a360>(item))
        return;
    UnkWeakTarget* actor = static_cast<UnkWeakTarget*>(item);
    if (IndexOf(&self->actors, actor) < 0)
        Append(&self->actors, actor);
    if (self->actors.count == 1)
        AssignRef(&self->current, GetWeakHandle(actor));
}
