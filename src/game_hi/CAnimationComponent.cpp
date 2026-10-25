// The actor's tracks (game/UnkAnimationComponent.h): the float at +0x60 of the action of the track
// looked up by name hash, or 0 when there is no such entry, track or action. The action getter is
// called twice, once for the test and once for the value, as in the original.
#include "game/UnkAnimationComponent.h"
#include "types.h"

f32 fn_7101177ea0(UnkAnimationComponent* self, u64 key) {
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
