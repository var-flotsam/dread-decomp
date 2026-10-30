// The actor's tracks (game/UnkAnimationComponent.h): the float at +0x60 of the action of the track
// looked up by name hash, or 0 when there is no such entry, track or action. The action getter is
// called twice, once for the test and once for the value, as in the original.
#include "game/UnkAnimationComponent.h"
#include "types.h"

static inline f32 ActionValue(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return 0.0f;
    UnkTrackEntry* entry = *it;
    if (!entry)
        return 0.0f;
    UnkTrack* track = entry->field_0x70;
    if (!track)
        return 0.0f;
    if (!fn_710070d500(track))
        return 0.0f;
    return fn_710070d500(track)->field_0x60;
}

f32 fn_7101177ea0(UnkAnimationComponent* self, u64 key) {
    return ActionValue(self, key);
}

// An identical copy next to it.
f32 fn_7101177f58(UnkAnimationComponent* self, u64 key) {
    return ActionValue(self, key);
}

// The word at +0x10 of the track's action, or null without one (generated, src/lifted/; the
// cutscene reads it as its timeline, src/game/CCutscene.cpp).
struct UnkCutsceneTimeline;
UnkCutsceneTimeline* fn_710070d4e8(UnkTrack* track);

// The word at +0x10 of the action of the track whose name hash is `key`, or null when there is no
// such entry or track.
void* fn_7101177d2c(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return nullptr;
    UnkTrackEntry* entry = *it;
    if (entry && entry->field_0x70)
        return fn_710070d4e8(entry->field_0x70);
    return nullptr;
}
