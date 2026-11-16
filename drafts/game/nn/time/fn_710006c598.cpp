// From unit 45 of tools/tu_map.py (named after nn::time): sets the point kept for a string id in a
// list of keyed 2D points, adding a node when the id has none, and notifies the listeners when the
// point moved away from the last one. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkClone_fn_710006ca34;
extern template void base::refl_DeferredInvokePtr<UnkClone_fn_710006ca34>(
    base::UnkDeferredSet<base::UnkDeferredCallback<const void*>>*, const void*);

struct UnkPoint_l06c598 {
    f32 x;
    f32 y;

    UnkPoint_l06c598(const UnkPoint_l06c598& other) : x(other.x), y(other.y) {}
    UnkPoint_l06c598& operator=(const UnkPoint_l06c598& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

struct UnkPointNode_l06c598 {
    base::global::CStrId key;
    UnkPoint_l06c598 point;  // 0x8
    f32 weight;              // 0x10
    UnkPointNode_l06c598* next;  // 0x18
};

struct UnkPointList_l06c598 {
    u8 pad_0x0[0x8];
    u32 count;                    // 0x8
    UnkPointNode_l06c598* head;   // 0x10
    UnkPointNode_l06c598* tail;   // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 alloc_flags;              // 0x28
    base::UnkDeferredSet<base::UnkDeferredCallback<const void*>> listeners;  // 0x30
};

// The point used when the list is empty.
__attribute__((visibility("hidden"))) extern const UnkPoint_l06c598 g_unk_710164768c;
// The 2D tests' tolerance (0.001).
extern const f32 g_unk_7101647774;

static inline bool IsOutside(f32 value, f32 centre) {
    return value < centre - g_unk_7101647774 || value > centre + g_unk_7101647774;
}

void fn_710006c598(UnkPointList_l06c598* self, const base::global::CStrId& key, const UnkPoint_l06c598& point) {
    UnkPoint_l06c598 last(self->count ? self->tail->point : g_unk_710164768c);

    UnkPointNode_l06c598* node = self->head;
    for (; node; node = node->next) {
        if (node->key == key)
            break;
    }
    if (node) {
        node->point = point;
        node->weight = -1.0f;
    } else {
        base::global::CStrId id(key);
        UnkPoint_l06c598 value(point);
        node = static_cast<UnkPointNode_l06c598*>(
            self->allocator->vf_0x40(sizeof(UnkPointNode_l06c598), 8, self->alloc_flags));
        if (node) {
            node->key.data = id.data;
            id.data = &g_unk_7101d47108->empty;
            node->point = value;
            node->weight = -1.0f;
            node->next = nullptr;
            if (self->count) {
                self->tail->next = node;
                node->next = nullptr;
                self->tail = node;
                self->count++;
            } else {
                self->head = node;
                self->tail = node;
                self->count = 1;
            }
        }
    }

    if (IsOutside(point.x, last.x) || IsOutside(point.y, last.y))
        base::refl_DeferredInvokePtr<UnkClone_fn_710006ca34>(&self->listeners, &point);
}
