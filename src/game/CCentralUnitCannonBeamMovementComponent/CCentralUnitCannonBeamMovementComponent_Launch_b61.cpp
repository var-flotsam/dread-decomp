// CCentralUnitCannonBeamMovementComponent's unit (tools/tu_map.py): after fn_7100ac3880, sets the launch event of
// the area kind at +0x5d8 ("onlaunch_cave", "_magma", "_lab", "_forest", "_sanc") on the owner's (+0x30) timeline
// (+0x2e8) to the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkSelf;
void fn_7100ac3880(UnkSelf* self);  // as src/game/CChozoCommanderEnergyShardsSphereMovementComponent/fn_7100459e6c.cpp

struct UnkCannonBeam_b61a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x5d8 - 0x38];
    u32 field_0x5d8;
};

__attribute__((always_inline)) static inline void Set_b61a2(UnkTimelineComponent* timeline, const char* name) {
    base::reflection::UnkReflTypeIdArg mask;
    base::global::CName id(name);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
}

void fn_71003ffb74(UnkCannonBeam_b61a2* self) {
    fn_7100ac3880(reinterpret_cast<UnkSelf*>(self));
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    switch (self->field_0x5d8) {
    case 0:
        Set_b61a2(timeline, "onlaunch_cave");
        break;
    case 1:
        Set_b61a2(timeline, "onlaunch_magma");
        break;
    case 2:
        Set_b61a2(timeline, "onlaunch_lab");
        break;
    case 3:
        Set_b61a2(timeline, "onlaunch_forest");
        break;
    case 4:
        Set_b61a2(timeline, "onlaunch_sanc");
        break;
    }
}
