// CCharClassPlayTimelineEvent (unit from tools/tu_map.py, the name from reflection): when the event's
// timeline id (+0x10) is set, the actor has a timeline component (+0x2e8) and the event's mode
// condition holds (fn_7100bc8e88), sets the timeline to the value of the track's action (+0x70), or
// to the all-flags mask when there is no track or action. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

template <u32 Offset>
struct UnkModeCondition;
struct UnkModeActor;
bool fn_7100bc8e88(const UnkModeCondition<0x18>* self, UnkModeActor* actor);

struct UnkTimelineNameData_b60 {
    base::global::UnkStringData base;
    u8 unk_0xc[0x24];
    u64 hash;
};

struct UnkPlayTimelineEvent_b60 {
    u8 unk_0x0[0x10];
    UnkTimelineNameData_b60* timeline;
};

struct UnkPlayTimelineTarget_b60 {
    UnkActor* actor;
    UnkTrack* track;
};

struct UnkTrackActionValue_b60 {
    u8 unk_0x0[0x70];
    u32 value;
};

void fn_7100bc8d50(UnkPlayTimelineEvent_b60* self, UnkPlayTimelineTarget_b60* target) {
    if (&self->timeline->base == &g_unk_7101d47108->empty)
        return;
    UnkActor* actor = target->actor;
    if (!actor)
        return;
    auto* timeline = static_cast<UnkTimelineComponent*>(actor->field_0x2e8);
    if (!timeline)
        return;
    if (!fn_7100bc8e88(reinterpret_cast<const UnkModeCondition<0x18>*>(self),
                       reinterpret_cast<UnkModeActor*>(actor)))
        return;
    u64 key = self->timeline->hash;
    const u32* value;
    if (target->track && fn_710070d500(target->track)) {
        value = &reinterpret_cast<UnkTrackActionValue_b60*>(fn_710070d500(target->track))->value;
    } else {
        value = &UnkAll_SpitSend().value;
        asm("" : "+r"(value));  // the original takes the static's address after its guard
    }
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = *value;
    fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
}
