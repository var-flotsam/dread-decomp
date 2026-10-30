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
// What `Get` reads from the track whose name hash is `key`, or zero.
template <class T, T (*Get)(UnkTrack*)>
static inline T TrackValue(UnkAnimationComponent* self, u64 key) {
    UnkTrackSet* set = self->field_0x40;
    UnkTrackEntry** it = set->field_0xc8.Find(key);
    if (it == set->field_0xc8.End())
        return T();
    UnkTrackEntry* entry = *it;
    if (entry && entry->field_0x70)
        return Get(entry->field_0x70);
    return T();
}

void* fn_7101177d2c(UnkAnimationComponent* self, u64 key) { return TrackValue<UnkCutsceneTimeline*, fn_710070d4e8>(self, key); }

// The same through two other getters of the track (a word and a float), next door.
u32 fn_710070d840(UnkTrack* track);
f32 fn_710070d850(UnkTrack* track);

u32 fn_7101177da8(UnkAnimationComponent* self, u64 key) { return TrackValue<u32, fn_710070d840>(self, key); }
f32 fn_7101177e24(UnkAnimationComponent* self, u64 key) { return TrackValue<f32, fn_710070d850>(self, key); }

// The first entry any of the component's action sets (+0x48, count at +0x50) holds under the pair
// of keys, or null (the action set's lookup is src/game/CActionSet.cpp's).
struct UnkActionSetMap;
void* fn_7100b8c61c(UnkActionSetMap* self, const u64* key1, const u64* key2);

struct UnkAnimationActionSets {
    u8 pad_0x0[0x48];
    UnkActionSetMap** sets;
    u32 count;
};

// NON_MATCHING(minor): the original counts the remaining bytes down before each call and returns the
// last call's result at the end (with its own null return for no sets); same behaviour.
void* fn_7101176e0c(UnkAnimationComponent* self, u64 key1, u64 key2) {
    UnkAnimationActionSets* a = reinterpret_cast<UnkAnimationActionSets*>(self);
    for (UnkActionSetMap **it = a->sets, **end = it + a->count; it != end; ++it)
        if (void* entry = fn_7100b8c61c(*it, &key1, &key2))
            return entry;
    return nullptr;
}
