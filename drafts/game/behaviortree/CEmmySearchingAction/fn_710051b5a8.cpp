// behaviortree::CEmmySearchingAction unit: drops from a list of candidate points those lying well
// behind the direction from the index-th tracked entry's target to the entry itself.
// Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "types.h"

#include <math.h>

struct UnkPos {
    f32 x, y;
    UnkPos() {}
    UnkPos(const UnkPos& o) : x(o.x), y(o.y) {}
};

struct UnkPoint {
    f32 x, y;
};

struct UnkTarget {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkPos vf_0x10();
    UnkPoint* field_0x8;
};

struct UnkEntry {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkPos vf_0x10();
    u8 pad_0x8[8];
    UnkTarget* field_0x10;
    u8 pad_0x18[8];
    UnkEntry* field_0x20;  // next
};

struct UnkEntryList {
    u32 count;
    UnkEntry* head;

    UnkEntry* At(u32 index) {
        if (index >= count)
            return nullptr;
        UnkEntry* entry = head;
        for (u32 n = count; index != 0; --index) {
            entry = entry->field_0x20;
            if (--n == 0)
                return nullptr;
        }
        return entry;
    }
};

struct UnkSearcher {
    u8 pad_0x0[0x12b08];
    UnkEntryList field_0x12b08;
};

void fn_710051b5a8(UnkSearcher* self, u32 index, base::UnkVector<UnkPoint*>* points) {
    UnkEntryList& entries = self->field_0x12b08;

    base::UnkAllocator* copy_allocator = points->allocator;
    UnkPoint** copy = nullptr;
    if (points->capacity)
        copy = static_cast<UnkPoint**>(
            copy_allocator->vf_0x40((u64)points->capacity * sizeof(UnkPoint*), 8, points->flags));
    __builtin_memcpy(copy, points->data, (u64)points->count * sizeof(UnkPoint*));
    u32 copy_count = points->count;

    UnkEntry* entry = entries.At(index);
    UnkTarget* target = entry->field_0x10;
    if (target) {
        f32 dir_y, dir_x;
        {
            UnkPos from = entry->vf_0x10();
            UnkPos to = entry->field_0x10->vf_0x10();
            dir_y = from.y - to.y;
            dir_x = from.x - to.x;
        }
        f32 len = sqrtf(dir_x * dir_x + dir_y * dir_y);
        if (len != 0.0f) {
            dir_x /= len;
            dir_y /= len;
        }

        base::UnkVector<f32> scores(0xf);
        f32 best = -1.0f;
        for (UnkPoint** it = points->data, **end = it + points->count; it != end; ++it) {
            UnkPoint* point = *it;
            UnkPos pos = entry->vf_0x10();
            f32 y = point->y - pos.y;
            f32 x = point->x - pos.x;
            f32 d = sqrtf(x * x + y * y);
            if (d != 0.0f) {
                x /= d;
                y /= d;
            }
            f32 score = dir_y * y + dir_x * x;
            best = score > best ? score : best;
            scores.PushBack(score);
        }
        f32 threshold = best - 1.3f;

        for (u32 i = 0; i < copy_count; ++i) {
            if (scores.GrowAt(i) < threshold && entry->field_0x10 && target->field_0x8 != copy[i])
                points->Remove(copy[i]);
        }
        if (scores.data)
            scores.allocator->vf_0x48(scores.data);
    }
    if (copy)
        copy_allocator->vf_0x48(copy);
}
