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

// The last segment that covers `t`; failing that, the last one when `t` is past the first's end,
// else the first one that starts at or after `t` (the end when none does).
UnkSegment* fn_7100c959bc(UnkSegmentList* self, f32 t) {
    if (self->count == 0)
        return nullptr;
    UnkSegment* first = self->entries;
    UnkSegment* last = first + self->count;
    UnkSegment* it = last;
    do {
        if (Covers(it[-1], t))
            break;
    } while (--it != first);
    if (it != first)
        return it - 1;
    if (first->end <= t)
        return last - 1;
    UnkSegment* p = first;
    do {
        if (p->start >= t)
            return p;
    } while (++p != last);
    return last;
}
