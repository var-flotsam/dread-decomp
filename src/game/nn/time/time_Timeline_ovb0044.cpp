// From unit 45 of tools/tu_map.py (named after nn::time; the timeline code of
// src/base/global/UnkTimelineTracks.cpp): searches over an array of timeline entries (a vtable, a
// 64-bit id, a start and an end time) by id and time, and the narrow-to-UTF-32 text conversion
// src/base/global/UnkWideTextMethods.cpp calls. Names and layouts are placeholders.
#include <algorithm>

#include "../../../game_s7/UnkIter.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

struct UnkTimelineEntry_ovb0044 {
    void* vtable;
    u64 id;     // 0x8
    f32 start;  // 0x10
    f32 end;    // 0x14
};

struct UnkTimelineEntries_ovb0044 : base::UnkVector<UnkTimelineEntry_ovb0044*> {
    UnkIter<UnkTimelineEntry_ovb0044*> begin() { return {data}; }
    UnkIter<UnkTimelineEntry_ovb0044*> end() { return {data + count}; }
};

// Whether the entry is the id's and covers `time`: from its start up to its end, the end included
// only when it is `frame` and that is `time` itself.
static inline bool Covers(const UnkTimelineEntry_ovb0044* e, u64 id, f32 time, s32 frame) {
    return e->id == id && (e->end > time || ((f32)frame == time && e->end == (f32)frame)) && e->start <= time;
}

// Whether an entry of the id covers the time.
bool fn_710006bacc(UnkTimelineEntries_ovb0044* self, u64 id, f32 time, s32 frame) {
    return std::find_if(self->begin(), self->end(), [&](const UnkTimelineEntry_ovb0044* e) {
               return Covers(e, id, time, frame);
           }) != self->end();
}

// The first entry of the id that covers the time, or null.
UnkTimelineEntry_ovb0044* fn_710006bb80(f32 time, UnkTimelineEntries_ovb0044* self, u64 id, s32 frame) {
    UnkIter<UnkTimelineEntry_ovb0044*> it = std::find_if(
        self->begin(), self->end(), [&](const UnkTimelineEntry_ovb0044* e) { return Covers(e, id, time, frame); });
    return it != self->end() ? *it : nullptr;
}

// A run of entries: the first and how many (returned in memory: its copy is user-written).
struct UnkTimelineRun_ovb0044 {
    UnkTimelineEntry_ovb0044** first;
    s64 count;

    UnkTimelineRun_ovb0044(UnkTimelineEntry_ovb0044** f, s64 n) : first(f), count(n) {}
    UnkTimelineRun_ovb0044(const UnkTimelineRun_ovb0044& o) : first(o.first), count(o.count) {}
};

// The first run of consecutive entries with the id (empty when there is none).
UnkTimelineRun_ovb0044 fn_710006b990(UnkTimelineEntries_ovb0044* self, u64 id) {
    auto has_id = [&](const UnkTimelineEntry_ovb0044* e) { return e->id == id; };
    UnkIter<UnkTimelineEntry_ovb0044*> first = std::find_if(self->begin(), self->end(), has_id);
    if (first == self->end())
        return UnkTimelineRun_ovb0044(nullptr, 0);
    UnkIter<UnkTimelineEntry_ovb0044*> last = std::find_if_not(first, self->end(), has_id);
    return UnkTimelineRun_ovb0044(first.p, last - first);
}
// The string's byte at an index.
static inline u8 ByteAt(const base::global::CRntString* s, s32 index) {
    return s->data[index];
}

// Converts a narrow string into at most `capacity` UTF-32 characters and terminates them. A byte 1
// escapes the next byte b: it stands for the character 0x3000 | (b - 1) (an escape at the very end is
// dropped).
void fn_710006c000(const base::global::CRntString* src, wchar_t* dst, u32 capacity) {
    if (!dst)
        return;
    u32 n = src->length < capacity ? src->length : capacity;
    s32 j = 0;
    for (u32 i = 0; i < n; ++i) {
        wchar_t c = ByteAt(src, i);
        if (c == 1) {
            if (++i >= src->length)
                continue;
            c = 0x3000 | (u8)(ByteAt(src, i) - 1);
        }
        // Stored untyped: the original reads the string again after each character.
        __builtin_memcpy(&dst[j++], &c, sizeof(c));
    }
    dst[j] = 0;
}
