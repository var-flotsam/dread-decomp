// Unit 2744 (gui/scripts/guimanager.lua): removes the entry for an object from a list of weak handles
// (the first whose handle points at the same object; head and tail are tried first). Names and layouts
// are placeholders.
#include "base/global/CRntVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A counted reference to a weak handle.
struct UnkWeakRef {
    UnkWeakHandle* handle;

    UnkWeakTarget* Get() const { return handle ? handle->target : nullptr; }
    bool operator==(const UnkWeakRef& other) const {
        UnkWeakTarget* target = Get();
        return other.Get() == target;
    }
    void Release() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
};

struct UnkWeakRefNode {
    UnkWeakRef value;
    UnkWeakRefNode* next;
    UnkWeakRefNode* prev;
};

struct UnkWeakRefList {
    u8 pad_0x0[8];
    s32 count;
    u8 pad_0xc[4];
    UnkWeakRefNode* head;
    UnkWeakRefNode* tail;
    u8 pad_0x20[8];
    base::UnkAllocator* allocator;
};
static_assert(__builtin_offsetof(UnkWeakRefList, allocator) == 0x28, "UnkWeakRefList placeholder layout");

void fn_71011b77e8(UnkWeakRefList* self, const UnkWeakRef* key) {
    UnkWeakRefNode* head = self->head;
    if (head && head->value == *key) {
        self->head = head->next;
        if (head->next)
            head->next->prev = nullptr;
        head->value.Release();
        self->allocator->vf_0x48(head);
        if (--self->count == 0)
            self->tail = self->head;
    } else if (UnkWeakRefNode* tail = self->tail; tail && tail->value == *key) {
        self->tail = tail->prev;
        if (tail->prev)
            tail->prev->next = nullptr;
        tail->value.Release();
        self->allocator->vf_0x48(tail);
        if (--self->count == 0)
            self->tail = self->head;
    } else {
        UnkWeakRefNode* node = head;
        while (node && !(node->value == *key))
            node = node->next;
        if (!node)
            return;
        node->prev->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
        --self->count;
        node->value.Release();
        self->allocator->vf_0x48(node);
    }
}
