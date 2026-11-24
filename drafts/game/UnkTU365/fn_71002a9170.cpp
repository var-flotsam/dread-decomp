// Unit 365 ("navmesh::CNavMeshGenerator"): reports a segment that is not flat to a callback; when
// the callback declines it and the segment faces along a direction, adds the reversed segment to a
// list for each polygon it leaves. Names and layouts are placeholders.
#include <math.h>
#include <functional>
#include <new>
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::math::CVector2D;

struct UnkEntry;
bool fn_7100293b58(const UnkSegment2D* segment, UnkEntry* entry);
struct UnkVec4fE;
bool fn_7100125b70(const UnkVec4fE* a, const UnkVec4fE* b);

// A list entry: a segment (its length not yet computed) and the next entry.
struct UnkSegNode_2a9170 {
    CVector2D start;
    CVector2D end;
    f32 length;
    u8 pad_0x14[4];
    UnkSegNode_2a9170* next;  // 0x18

    UnkSegNode_2a9170(const UnkSegment2D& seg) : start(seg.start), end(seg.end), length(-1.0f), next(nullptr) {}
};

struct UnkSegList_2a9170 {
    u8 pad_0x0[8];
    u32 count;  // 0x8
    u8 pad_0xc[4];
    UnkSegNode_2a9170* head;  // 0x10
    UnkSegNode_2a9170* tail;  // 0x18
    base::IAllocator* allocator;  // 0x20
    u32 flags;  // 0x28
};

// A polygon record (0x48 bytes).
struct UnkPolygon_2a9170 {
    u8 data[0x48];
};

struct UnkReporter_2a9170 {
    std::function<bool(const UnkSegment2D&, u32, CRntString)>* callback;  // 0x0
    UnkSegList_2a9170** segments;  // 0x8
    u8 pad_0x10[8];
    base::global::CRntVector<UnkPolygon_2a9170>* polygons;  // 0x18
    u32* depth;  // 0x20
    const CRntString* indent;  // 0x28
};

static inline bool IsNearZero(f32 v) {
    return v - 0.001f <= 0.0f && v + 0.001f >= 0.0f;
}

// Adds the segment to the list unless it is already there.
static inline void AddUnique(UnkSegList_2a9170** list, const UnkSegment2D& seg) {
    for (UnkSegNode_2a9170* node = (*list)->head; node != nullptr; node = node->next) {
        if (fn_7100125b70(reinterpret_cast<const UnkVec4fE*>(node), reinterpret_cast<const UnkVec4fE*>(&seg))) {
            return;
        }
    }
    UnkSegList_2a9170* l = *list;
    void* memory = l->allocator->vf_0x40(sizeof(UnkSegNode_2a9170), 8, l->flags);
    if (memory == nullptr) {
        return;
    }
    UnkSegNode_2a9170* node = new (memory) UnkSegNode_2a9170(seg);
    if (l->count == 0) {
        l->head = node;
        l->tail = node;
    } else {
        l->tail->next = node;
        node->next = nullptr;
        l->tail = node;
    }
    l->count++;
}

void fn_71002a9170(UnkReporter_2a9170* self, const UnkSegment2D* seg, bool flip, const CVector2D* dir, bool add,
                   f32 cross) {
    if (IsNearZero(cross)) {
        return;
    }
    if ((*self->callback)(*seg, *self->depth, CRntString_ConcatCString(self->indent, "  "))) {
        return;
    }
    if (!add) {
        return;
    }
    CVector2D d = {seg->end.x - seg->start.x, seg->end.y - seg->start.y};
    f32 len = sqrtf(d.x * d.x + d.y * d.y);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        d.x *= inv;
        d.y *= inv;
    }
    f32 sign = flip ? -1.0f : 1.0f;
    if ((sign * d.x) * dir->x + (sign * d.y) * dir->y <= 0.0f) {
        return;
    }
    for (size_t i = 0; i < *self->depth && i < self->polygons->count; i++) {
        if (!fn_7100293b58(seg, reinterpret_cast<UnkEntry*>(&self->polygons->data[i]))) {
            continue;
        }
        UnkSegList_2a9170** segments = self->segments;
        UnkSegment2D reversed(seg->end, seg->start);
        AddUnique(segments, reversed);
    }
}
