// Unit 2744 (gui/scripts/guimanager.lua): the GUI root is told that an object gained the property
// game/UnkGuiDisplayObject.h describes: unless its list already refers to the object, a weak
// reference to it goes into the list, ordered by the objects' priority (+0x138; negative ones go
// last). Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

namespace {

// A GUI object as the list orders it.
struct UnkGuiOrderedObject : UnkWeakTarget {
    u8 pad_0x8[0x130];
    s32 priority;
};
static_assert(__builtin_offsetof(UnkGuiOrderedObject, priority) == 0x138, "UnkGuiOrderedObject placeholder layout");

// A counted reference held for the duration of the call.
struct UnkHeldHandle {
    UnkWeakHandle* handle;

    explicit UnkHeldHandle(UnkWeakHandle* h) : handle(h) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHeldHandle() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

struct UnkGuiOrderedNode {
    UnkWeakHandle* handle;
    UnkGuiOrderedNode* next;
    UnkGuiOrderedNode* prev;

    UnkWeakTarget* Target() const { return handle ? handle->target : nullptr; }
};

struct UnkGuiOrderedList {
    u32 count;
    u8 pad_0x4[4];
    UnkGuiOrderedNode* head;
    UnkGuiOrderedNode* tail;
    u32 tag;
    u8 pad_0x1c[4];
    base::IAllocator* allocator;

    UnkGuiOrderedNode* NewNode(UnkWeakHandle* handle) {
        UnkGuiOrderedNode* node =
            static_cast<UnkGuiOrderedNode*>(allocator->vf_0x40(sizeof(UnkGuiOrderedNode), 8, tag));
        if (node) {
            node->handle = nullptr;
            AssignRef(&node->handle, handle);
            node->next = nullptr;
            node->prev = nullptr;
        }
        return node;
    }

    void PushBack(UnkWeakHandle* handle) {
        UnkGuiOrderedNode* node = NewNode(handle);
        if (!node)
            return;
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            UnkGuiOrderedNode*& last_next = tail->next;
            last_next = node;
            node->prev = tail;
            tail = node;
            count++;
        }
    }

    void InsertBefore(UnkGuiOrderedNode* at, UnkWeakHandle* handle) {
        UnkGuiOrderedNode* node = NewNode(handle);
        if (!node)
            return;
        UnkGuiOrderedNode* prev = at->prev;
        at->prev = node;
        node->next = at;
        node->prev = prev;
        (prev ? prev->next : head) = node;
        count++;
    }
};

struct UnkGuiRootOrderedView {
    u8 pad_0x0[0x1c0];
    UnkGuiOrderedList list;
};
static_assert(__builtin_offsetof(UnkGuiRootOrderedView, list) == 0x1c0, "UnkGuiRootOrderedView placeholder layout");

}  // namespace

void fn_71011b7514(UnkGuiRoot* root, UnkGuiDisplayObject* object) {
    UnkGuiOrderedList& list = reinterpret_cast<UnkGuiRootOrderedView*>(root)->list;
    UnkGuiOrderedObject* target = reinterpret_cast<UnkGuiOrderedObject*>(object);
    UnkHeldHandle held(GetWeakHandle(target));

    UnkGuiOrderedNode* found = list.head;
    while (found && found->Target() != (held.handle ? held.handle->target : nullptr))
        found = found->next;
    if (found)
        return;

    s32 priority = target->priority;
    if (priority >= 0) {
        for (UnkGuiOrderedNode* node = list.head; node; node = node->next) {
            s32 other = static_cast<UnkGuiOrderedObject*>(node->handle->target)->priority;
            if (other < 0 || other >= priority) {
                list.InsertBefore(node, held.handle);
                return;
            }
        }
    }
    list.PushBack(held.handle);
}
