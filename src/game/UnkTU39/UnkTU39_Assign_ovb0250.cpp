// Unit 39 of tools/tu_map.py (no reliable name): the copy assignment of a keyed list whose callback set is told
// (with false) when the list becomes empty: releases every node (id and memory), then appends a copy of each of
// the source's nodes, using the list's own allocator and tag. Returns the list. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU39.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CStrId;

void fn_71000303c0(UnkDeferredSet<UnkDeferredCallback<bool>>* s, const bool* arg);

struct UnkKeyNode_b0250 {
    CStrId id;
    f32 value;
    UnkKeyNode_b0250* next;
    UnkKeyNode_b0250(const UnkKeyNode_b0250* src) : id(src->id) {
        value = src->value;
        next = nullptr;
    }
};

struct UnkKeyList_b0250 {
    void** vtable;
    u32 count;
    UnkKeyNode_b0250* head;
    UnkKeyNode_b0250* tail;
    base::UnkAllocator* allocator;
    u32 tag;
    UnkDeferredSet<UnkDeferredCallback<bool>> on_change;  // 0x30
};

void* fn_710002f90c(void* dst, const void* src) {
    UnkKeyList_b0250* self = static_cast<UnkKeyList_b0250*>(dst);
    const UnkKeyList_b0250* other = static_cast<const UnkKeyList_b0250*>(src);
    UnkKeyNode_b0250* node = self->head;
    u32 old_count = self->count;
    if (node) {
        UnkKeyNode_b0250* next = node->next;
        for (;;) {
            node->id.Reset();
            self->allocator->vf_0x48(node);
            if (!next)
                break;
            node = next;
            next = next->next;
        }
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    if (old_count != 0) {
        bool arg = false;
        fn_71000303c0(&self->on_change, &arg);
    }
    for (const UnkKeyNode_b0250* n = other->head; n; n = n->next) {
        void* mem = self->allocator->vf_0x40(sizeof(UnkKeyNode_b0250), 8, self->tag);
        if (!mem)
            continue;
        UnkKeyNode_b0250* copy = new (mem) UnkKeyNode_b0250(n);
        u32 new_count;
        if (self->count == 0) {
            self->head = copy;
            self->tail = copy;
            new_count = 1;
        } else {
            UnkKeyNode_b0250* last = self->tail;
            __builtin_memcpy(&last->next, &copy, sizeof(copy));
            copy->next = nullptr;
            self->tail = copy;
            new_count = self->count + 1;
        }
        self->count = new_count;
    }
    return self;
}
