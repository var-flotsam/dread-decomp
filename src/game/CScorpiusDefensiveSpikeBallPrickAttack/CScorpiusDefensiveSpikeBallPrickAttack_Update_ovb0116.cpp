// CScorpiusDefensiveSpikeBallPrickAttack's unit (tools/tu_map.py): the attack's update. The target's
// melee override (+0x2f8 of the actor fn_7100d7c990 gives) is set from the attacker's character
// class (+0x220): when it has a CCharClassOverrideSamusMeleeActionTrack (fn_710117b348) the
// override's flag (+0x1e8) is set and it takes the track's string id (fn_710117b08c's record,
// +0x18); otherwise the flag is cleared. Then the base update runs (fn_710068685c). Names and
// layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAttack.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkMeleeOverride_sdp {
    u8 pad_0x0[0x1e8];
    bool active;  // 0x1e8
    u8 pad_0x1e9[7];
    CStrId action;  // 0x1f0
};

struct UnkMeleeTarget_sdp {
    u8 pad_0x0[0x2f8];
    UnkMeleeOverride_sdp* melee_override;  // 0x2f8
};

void fn_710068685c(CAttack* self, f32 dt);

void fn_7100687170(CAttack* self, f32 dt) {
    UnkMeleeTarget_sdp* target = reinterpret_cast<UnkMeleeTarget_sdp*>(fn_7100d7c990(self->field_0x8));
    UnkMeleeOverride_sdp* melee = target->melee_override;
    UnkAnimationComponent* anim = static_cast<UnkAnimationComponent*>(self->field_0x8->field_0x220);
    if (melee && anim && fn_710117b348(anim, fn_7100c44688())) {
        UnkTrackQuery query = {fn_7100c44688(), nullptr, -1.0f};
        fn_710117b08c(anim, &query);
        UnkTrackRecord* record = query.result;
        melee->active = true;
        melee->action = record->sources;
    } else {
        melee->active = false;
    }
    fn_710068685c(self, dt);
}
