// Unit 39 of tools/tu_map.py (no reliable name): the copy constructor of the keyed list (vtable, count, head, tail,
// allocator and tag, then a deferred callback set at +0x30; nodes of {string id, float, float, next} from the
// allocator). Each node of the source is copied (its id shared, its first float taken) and appended; the
// callback set at +0x30 is then moved over by fn_710003017c (src/game/UnkTU39/UnkTU39_SetMove_ovb0250.cpp).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::UnkDeferredCallback;
using base::UnkDeferredSet;
using base::global::CStrId;

extern void* g_unk_7101a1a090[];

struct UnkKeyNode_b0250 {
    CStrId id;
    f32 value;
    f32 value2;
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
    UnkDeferredSet<UnkDeferredCallback<f32>> on_change;  // 0x30
};

void fn_710003017c(UnkDeferredSet<UnkDeferredCallback<f32>>* self, UnkDeferredSet<UnkDeferredCallback<f32>>* other);

void fn_710002f588(void* dst, const void* src) {
    UnkKeyList_b0250* self = static_cast<UnkKeyList_b0250*>(dst);
    const UnkKeyList_b0250* other = static_cast<const UnkKeyList_b0250*>(src);
    self->count = 0;
    self->vtable = &g_unk_7101a1a090[2];
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = other->allocator;
    self->tag = other->tag;
    for (const UnkKeyNode_b0250* n = other->head; n; n = n->next) {
        void* mem = self->allocator->vf_0x40(sizeof(UnkKeyNode_b0250), 8, self->tag);
        if (!mem)
            continue;
        UnkKeyNode_b0250* node = new (mem) UnkKeyNode_b0250(n);
        u32 new_count;
        if (self->count == 0) {
            self->head = node;
            self->tail = node;
            new_count = 1;
        } else {
            UnkKeyNode_b0250* last = self->tail;
            __builtin_memcpy(&last->next, &node, sizeof(node));
            node->next = nullptr;
            self->tail = node;
            new_count = self->count + 1;
        }
        self->count = new_count;
    }
    fn_710003017c(&self->on_change, const_cast<UnkDeferredSet<UnkDeferredCallback<f32>>*>(&other->on_change));
}
