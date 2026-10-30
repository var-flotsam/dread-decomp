// A lookup in the CAnimationComponent unit (unit 2720 of tools/tu_map.py): through the handle's object
// at +0x40, the entry the hash `key` names in its map of parallel arrays at +0xc8
// (base/UnkParallelFind.h), and that entry's track at +0x70; null when there is none. Names and
// layouts are placeholders.
#include "base/UnkParallelFind.h"

struct UnkTrack;

struct UnkAnimEntry {
    u8 pad_0x0[0x70];
    UnkTrack* track;
};

struct UnkAnimEntryMap {
    u8 pad_0x0[0xc8];
    UnkAnimEntry** values;  // 0xc8
    u64* ids;               // 0xd0
    u32 count;              // 0xd8
};

struct UnkAnimHandle {
    u8 pad_0x0[0x40];
    UnkAnimEntryMap* map;
};

static inline UnkTrack* FindTrack(void* handle, u64 key) {
    UnkAnimEntry* entry = UnkParallelFindByValue(static_cast<UnkAnimHandle*>(handle)->map, key);
    return entry ? entry->track : nullptr;
}

UnkTrack* fn_71011771e0(void* handle, u64 key) { return FindTrack(handle, key); }
// A copy next door.
UnkTrack* fn_7101177b68(void* handle, u64 key) { return FindTrack(handle, key); }
// Hands that track (null when there is none) to fn_710070d5dc, next door.
void fn_710070d5dc(u64 value);
void fn_7101177a70(void* handle, u64 key) { fn_710070d5dc(reinterpret_cast<u64>(FindTrack(handle, key))); }
