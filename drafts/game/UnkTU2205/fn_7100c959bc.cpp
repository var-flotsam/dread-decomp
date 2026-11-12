// Unit 2205: finds the entry of a time-keyed array that covers a given time.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSegment {
    f32 start;
    f32 end;
    u8 pad_0x8[0x20];
};

static inline bool Covers(const UnkSegment& seg, f32 t) {
    f32 lo = seg.start;
    f32 hi = seg.end;
    return lo <= t && hi > t;
}

struct UnkSegmentList {
    u8 pad_0x0[8];
    UnkSegment* entries;
    u32 count;
};

UnkSegment* fn_7100c959bc(UnkSegmentList* self, f32 t) {
    u64 count = self->count;
    if (count == 0)
        return nullptr;
    UnkSegment* first = self->entries;
    UnkSegment* last = first + count;
    UnkSegment* base = first - 1;
    for (u64 i = count; i != 0; --i) {
        UnkSegment* e = base + i;
        if (Covers(*e, t)) {
            if (i != 1)
                return e;
            break;
        }
    }
    if (first->end <= t)
        return last - 1;
    if (first->start >= t)
        return first;
    UnkSegment* p = first;
    for (u64 n = count - 1; n != 0; --n) {
        p = p + 1;
        if (p->start >= t)
            return p;
    }
    return last;
}
