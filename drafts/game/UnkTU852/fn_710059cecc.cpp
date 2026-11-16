// Unit 852: removes the node whose counted weak reference points at the same object as `ref` from
// a singly linked list, dropping the node's reference and freeing it. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU852.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// The object a counted weak-handle reference points at, or null.
static inline UnkWeakTarget* RefTarget(const UnkRefPtr& ref) {
    auto* handle = static_cast<UnkWeakHandle*>(ref.field_0x0);
    return handle ? handle->target : nullptr;
}

struct UnkRefNode_l59cecc {
    UnkRefPtr ref;
    UnkRefNode_l59cecc* next;
};

struct UnkRefList_l59cecc {
    void** vtable;
    u32 count;
    UnkRefNode_l59cecc* head;
    UnkRefNode_l59cecc* tail;
    base::UnkAllocator* allocator;

    UnkRefNode_l59cecc* Find(const UnkRefPtr& key) {
        for (UnkRefNode_l59cecc* n = head; n; n = n->next) {
            if (RefTarget(n->ref) == RefTarget(key))
                return n;
        }
        return nullptr;
    }

    void Erase(UnkRefNode_l59cecc* node) {
        UnkRefNode_l59cecc* prev = nullptr;
        UnkRefNode_l59cecc* cur = head;
        UnkRefNode_l59cecc* next;
        UnkRefNode_l59cecc* last = nullptr;
        for (;;) {
            if (!cur)
                return;
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
        node->ref.~UnkRefPtr();
        allocator->vf_0x48(node);
    }
};

void fn_710059cecc(UnkHandleList* list, const UnkRefPtr& ref) {
    auto* self = reinterpret_cast<UnkRefList_l59cecc*>(list);
    if (UnkRefNode_l59cecc* node = self->Find(ref))
        self->Erase(node);
}