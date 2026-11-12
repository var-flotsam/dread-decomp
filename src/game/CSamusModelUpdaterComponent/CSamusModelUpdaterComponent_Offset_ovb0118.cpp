// CSamusModelUpdaterComponent's unit (tools/tu_map.py): fn_710093ab9c, the model offset: while the
// owner's animation has a CCharClassApplyGhostDashModelOffsetTrack record with a positive offset and the
// game manager has its ability (fn_7100df554c), the ability's offset (fn_7100313074) for what is left of
// the track's span; plus the height at +0x5324 of the owner's +0x208. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkAbility;
UnkVec3f fn_7100313074(UnkAbility* ability, f32 amount);

// The ghost-dash offset track's record: the time span (+0x10, +0x14) and the offset (+0x18).
struct UnkGhostDashTrack_ovb0118 {
    u8 pad_0x0[0x10];
    f32 start;  // 0x10
    f32 end;    // 0x14
    f32 offset;  // 0x18
};

struct UnkSamusModelOffset_ovb0118 {
    u8 pad_0x0[0x5324];
    f32 height;  // 0x5324
};

struct UnkSamusModelOwner_ovb0118 {
    u8 pad_0x0[0x208];
    UnkSamusModelOffset_ovb0118* field_0x208;
    u8 pad_0x210[0x10];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkSamusModelUpdater_ovb0118 {
    u8 pad_0x0[0x30];
    UnkSamusModelOwner_ovb0118* owner;  // 0x30
};

void fn_710093ab9c(UnkSamusModelUpdater_ovb0118* self, UnkVec3f* offset) {
    if (self->owner->animation) {
        UnkTrackQuery query;
        query.cls = fn_7100cebf38();
        query.result = nullptr;
        query.value = -1.0f;
        fn_710117b08c(self->owner->animation, &query);
        UnkGhostDashTrack_ovb0118* track = reinterpret_cast<UnkGhostDashTrack_ovb0118*>(query.result);
        if (track && track->offset > 0.0f) {
            if (UnkAbility* ability = fn_7100df554c(g_unk_7101d47310)) {
                f32 t = fn_7101177f58(self->owner->animation, 0xf4061ff25a2163d6);
                track = reinterpret_cast<UnkGhostDashTrack_ovb0118*>(query.result);
                f32 remaining = 1.0f - (t - track->start) / (track->end - track->start);
                *offset = fn_7100313074(ability, track->offset * remaining);
            }
        }
    }
    if (UnkSamusModelOffset_ovb0118* extra = self->owner->field_0x208)
        offset->y += extra->height;
}
