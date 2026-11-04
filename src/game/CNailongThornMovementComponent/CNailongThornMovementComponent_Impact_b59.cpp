// CNailongThornMovementComponent's unit (tools/tu_map.py): two impact handlers with one body. The first
// time (flag at +0x5d9), when the owner (+0x30) has its timeline (+0x2e8), each sets the id hashed
// from "onimpact" to the all-flags mask (as the acid ball's,
// src/game/CNailuggerAcidBallMovementComponent/CNailuggerAcidBallMovementComponent_Impact_b59.cpp), and
// answers true. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkThorn_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x5d9 - 0x38];
    bool field_0x5d9;
};

static inline bool SendOnImpactOnce_b59a4(UnkThorn_b59a4* self) {
    if (!self->field_0x5d9) {
        self->field_0x5d9 = true;
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            base::global::CName name("onimpact");
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &name.hash, &mask.id, false, -1.0f);
        }
    }
    return true;
}

bool fn_71006232d0(UnkThorn_b59a4* self) {
    return SendOnImpactOnce_b59a4(self);
}

bool fn_71006233dc(UnkThorn_b59a4* self) {
    return SendOnImpactOnce_b59a4(self);
}
