// CCentralUnitCannonBeamMovementComponent's unit (tools/tu_map.py): after fn_7100ac4dc4, sets the impact event of
// the area kind at +0x5d8 ("onimpact_cave", "_magma", "_lab", "_forest", "_sanc") on the owner's (+0x30) timeline
// (+0x2e8) to the all-flags mask and records it at +0x524. Same shape as fn_71003ffb74. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkCannonBeam_b62a2;
void fn_7100ac4dc4(UnkCannonBeam_b62a2* self);

struct UnkCannonBeam_b62a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x524 - 0x38];
    bool field_0x524;
    u8 pad_0x525[0x5d8 - 0x525];
    u32 field_0x5d8;
};

__attribute__((always_inline)) static inline void SetImpact_b62a2(UnkCannonBeam_b62a2* self,
                                                                  UnkTimelineComponent* timeline, const char* name) {
    base::reflection::UnkReflTypeIdArg mask;
    base::global::CName id(name);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
    self->field_0x524 = true;
}

void fn_7100400818(UnkCannonBeam_b62a2* self) {
    fn_7100ac4dc4(self);
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    switch (self->field_0x5d8) {
    case 0:
        SetImpact_b62a2(self, timeline, "onimpact_cave");
        break;
    case 1:
        SetImpact_b62a2(self, timeline, "onimpact_magma");
        break;
    case 2:
        SetImpact_b62a2(self, timeline, "onimpact_lab");
        break;
    case 3:
        SetImpact_b62a2(self, timeline, "onimpact_forest");
        break;
    case 4:
        SetImpact_b62a2(self, timeline, "onimpact_sanc");
        break;
    }
}

