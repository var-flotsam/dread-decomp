// Lookups in the 0x40-byte entries keyed by 8-byte ids at +0x68 of an object in CTimelineComponent's
// unit (base::UnkKeyedArray, as fn_710079c41c's lookup in
// src/game/CActorComponent/fn_71002519a8.cpp). Names and the layout are placeholders.
#include "base/UnkCloneCalls.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkTimelineEntry {
    UnkTrack* track;
    u8 unk_0x8[0x38];
};
struct UnkTimelineEntries_7100a2cfe8;

struct UnkTimelineOwner {
    u8 pad_0x0[0x68];
    base::UnkKeyedArray<UnkTimelineEntries_7100a2cfe8, u64, UnkTimelineEntry> entries;
};

f32 fn_710070d89c(UnkTrack* track);
u32 fn_710070d840(UnkTrack* track);

bool fn_7100a2cfe8(UnkTimelineOwner* self, const u64* key) {
    return self->entries.Find(*key) != self->entries.End();
}

// A value read from the track of the entry a key names (fn_710070d89c, fn_710070d840), or zero
// when the key is absent.
f32 fn_7100a2d4d8(UnkTimelineOwner* self, const u64* key) {
    UnkTimelineEntry* e = self->entries.Find(*key);
    return e != self->entries.End() ? fn_710070d89c(e->track) : 0.0f;
}

u32 fn_7100a2d54c(UnkTimelineOwner* self, const u64* key) {
    UnkTimelineEntry* e = self->entries.Find(*key);
    return e != self->entries.End() ? fn_710070d840(e->track) : 0;
}
