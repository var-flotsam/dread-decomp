// CTimelineComponent unit (tools/tu_map.py): follows the owner animation's current
// CCharClassPlayTimelineTrack record: stops the timeline of the one kept at +0xe8 when it no longer
// applies, and starts the new one's. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineNameData_ovb0089 {
    u8 pad_0x0[0x30];
    u64 hash;
};

// A CCharClassPlayTimelineTrack record: its track name (+0x18) and the timeline it plays (+0x28).
struct UnkPlayTimelineTrack_ovb0089 {
    u8 pad_0x0[0x18];
    UnkTimelineNameData_ovb0089* name;
    u8 pad_0x20[0x8];
    UnkTimelineNameData_ovb0089* timeline;
};

struct UnkTimelineOwnerActor_ovb0089 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkTimelinePlayer_ovb0089 {
    u8 pad_0x0[0x30];
    UnkTimelineOwnerActor_ovb0089* owner;
    u8 pad_0x38[0xe8 - 0x38];
    UnkPlayTimelineTrack_ovb0089* current;  // 0xe8
};

bool fn_7100c45dc8(UnkPlayTimelineTrack_ovb0089* track, UnkTimelineOwnerActor_ovb0089* owner);

static inline void Play(UnkTimelinePlayer_ovb0089* self, u64* key, u32* mask, u64 id) {
    *key = id;
    *mask = base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>();
    fn_7100a2cb7c(reinterpret_cast<UnkTimelineComponent*>(self), key, mask, false, -1.0f);
}

void fn_7100a2b564(UnkTimelinePlayer_ovb0089* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;
    alignas(8) u32 mask;
    UnkTrackQuery query;
    u64 key;
    query.cls = fn_7100c455b8();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(animation, &query);
    auto* track = reinterpret_cast<UnkPlayTimelineTrack_ovb0089*>(query.result);
    if (UnkPlayTimelineTrack_ovb0089* current = self->current) {
        bool valid = fn_7100c45dc8(current, self->owner);
        if (!track || !valid || track->name->hash != self->current->name->hash) {
            Play(self, &key, &mask, self->current->timeline->hash);
            self->current = nullptr;
        }
    }
    if (!track)
        return;
    if (!fn_7100c45dc8(track, self->owner))
        return;
    if (self->current && self->current->name->hash == track->name->hash)
        return;
    Play(self, &key, &mask, track->name->hash);
    self->current = track;
}
