// CPlaythrough's unit (tools/tu_map.py): replaces an owning pointer with the one at +8 of `other` and deletes
// the previous entry (its owned object at +0x30, then its string id at +8, released as base::file::
// UnkDeviceName). Names and layouts are placeholders.
#include "base/CDeviceQueue.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

struct UnkPlaythroughEntry_A2_b70 {
    u8 pad_0x0[0x8];
    base::file::UnkDeviceName name;  // 0x8
    u8 pad_0x10[0x20];
    UnkOwnedPtr owned;  // 0x30
};

struct UnkPlaythroughSource_A2_b70 {
    u8 pad_0x0[0x8];
    UnkPlaythroughEntry_A2_b70* entry;
};

void fn_71002eccc0(UnkPlaythroughEntry_A2_b70** self, UnkPlaythroughSource_A2_b70* other) {
    UnkPlaythroughEntry_A2_b70* entry = other->entry;
    UnkPlaythroughEntry_A2_b70* old = *self;
    *self = entry;
    if (old)
        delete old;
}
