// From unit 45 of tools/tu_map.py (named after nn::time): removes the point kept for a string id
// from a list of keyed 2D points and notifies the listeners when the last point moved because of
// it. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkClone_fn_710006ca34;
extern template void base::refl_DeferredInvokePtr<UnkClone_fn_710006ca34>(
    base::UnkDeferredSet<base::UnkDeferredCallback<const void*>>*, const void*);

struct UnkPoint_l06c76c {
    f32 x;
    f32 y;

    UnkPoint_l06c76c(const UnkPoint_l06c76c& other) : x(other.x), y(other.y) {}
};

struct UnkPointNode_l06c76c {
    base::global::CStrId key;
    UnkPoint_l06c76c point;      // 0x8
    f32 weight;                  // 0x10
    UnkPointNode_l06c76c* next;  // 0x18
};

struct UnkPointList_l06c76c {
    u8 pad_0x0[0x8];
    u32 count;                      // 0x8
    UnkPointNode_l06c76c* head;     // 0x10
    UnkPointNode_l06c76c* tail;     // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 alloc_flags;                // 0x28
    base::UnkDeferredSet<base::UnkDeferredCallback<const void*>> listeners;  // 0x30
};

// The point used when the list is empty.
__attribute__((visibility("hidden"))) extern const UnkPoint_l06c76c g_unk_710164768c;
// The 2D tests' tolerance (0.001).
extern const f32 g_unk_7101647774;

static inline bool IsOutside(f32 value, f32 centre) {
    return value < centre - g_unk_7101647774 || value > centre + g_unk_7101647774;
}

// The last point, or the default one when the list is empty.
static inline UnkPoint_l06c76c LastPoint(UnkPointList_l06c76c* list) {
    if (list->count)
        return list->tail->point;
    return g_unk_710164768c;
}

// Unlinks a node (found from the head), releases its id and frees it.
static inline void EraseNode(UnkPointList_l06c76c* list, UnkPointNode_l06c76c* node) {
    UnkPointNode_l06c76c* last = nullptr;
    UnkPointNode_l06c76c* it = list->head;
    while (it) {
        UnkPointNode_l06c76c* prev = last;
        last = it;
        it = it->next;
        if (last != node)
            continue;
        if (prev) {
            prev->next = it;
            if (list->tail == node)
                list->tail = prev;
        } else {
            list->head = it;
        }
        if (--list->count == 0) {
            list->head = nullptr;
            list->tail = nullptr;
        }
        node->key.Reset();
        list->allocator->vf_0x48(node);
        return;
    }
}

void fn_710006c76c(UnkPointList_l06c76c* self, const base::global::CStrId& key) {
    UnkPoint_l06c76c before = LastPoint(self);

    UnkPointNode_l06c76c* node = self->head;
    while (node && !(node->key == key))
        node = node->next;
    if (node)
        EraseNode(self, node);

    UnkPoint_l06c76c now(LastPoint(self));
    if (IsOutside(now.x, before.x) || IsOutside(now.y, before.y))
        base::refl_DeferredInvokePtr<UnkClone_fn_710006ca34>(&self->listeners, &now);
}
