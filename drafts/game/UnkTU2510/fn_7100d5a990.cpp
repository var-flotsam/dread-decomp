// Unit 2510: finds the slot of the tracker's pointer array (+0x8, count at +0x10) whose entry is set and
// refers to `object` (+0x10); the end slot when there is none. Names and layouts are placeholders.
#include <algorithm>

#include "types.h"

struct UnkTrackedObject_ld5a990;

struct UnkTrackedEntry_ld5a990 {
    u8 pad_0x0[0x10];
    UnkTrackedObject_ld5a990* object;  // 0x10
};

struct UnkTracker_ld5a990 {
    u8 pad_0x0[8];
    UnkTrackedEntry_ld5a990** data;  // 0x8
    u32 count;                       // 0x10
};

UnkTrackedEntry_ld5a990** fn_7100d5a990(UnkTracker_ld5a990* tracker, UnkTrackedObject_ld5a990* object) {
    UnkTrackedEntry_ld5a990** begin = tracker->data;
    UnkTrackedEntry_ld5a990** end = begin + tracker->count;
    return std::find_if(begin, end, [object](const UnkTrackedEntry_ld5a990* entry) {
        return entry && entry->object == object;
    });
}
