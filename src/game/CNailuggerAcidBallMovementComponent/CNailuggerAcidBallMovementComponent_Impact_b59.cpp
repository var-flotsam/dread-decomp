// CNailuggerAcidBallMovementComponent's unit (tools/tu_map.py): two impact handlers with one body.
// When the owner (+0x30) has its timeline (+0x2e8), each sets the id hashed from "onimpact" to the
// all-flags mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp), and answers true. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkAcidBall_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

static inline bool SendOnImpact_b59a4(UnkAcidBall_b59a4* self) {
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        base::reflection::UnkReflTypeIdArg mask;
        base::global::CName name("onimpact");
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &name.hash, &mask.id, false, -1.0f);
    }
    return true;
}

bool fn_710062671c(UnkAcidBall_b59a4* self) {
    return SendOnImpact_b59a4(self);
}

bool fn_7100626818(UnkAcidBall_b59a4* self) {
    return SendOnImpact_b59a4(self);
}
