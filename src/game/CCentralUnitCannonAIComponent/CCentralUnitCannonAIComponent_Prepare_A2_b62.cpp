// CCentralUnitCannonAIComponent's unit (tools/tu_map.py): clears +0x146c4 and sets the attack-preparation event of
// the area kind at +0x146cc ("attackpreparation_cave", "_magma", "_lab", "_forest", "_sanc") on the owner's (+0x30)
// timeline (+0x2e8) to the all-flags mask. Same shape as fn_71003ffb74. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkCannonAI_b62a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c4 - 0x38];
    u32 field_0x146c4;
    u32 pad_0x146c8;
    u32 field_0x146cc;
};

__attribute__((always_inline)) static inline void Set_b62a2(UnkTimelineComponent* timeline, const char* name) {
    base::reflection::UnkReflTypeIdArg mask;
    base::global::CName id(name);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id.hash, &mask.id, false, -1.0f);
}

void fn_71003fd320(UnkCannonAI_b62a2* self) {
    self->field_0x146c4 = 0;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    switch (self->field_0x146cc) {
    case 0:
        Set_b62a2(timeline, "attackpreparation_cave");
        break;
    case 1:
        Set_b62a2(timeline, "attackpreparation_magma");
        break;
    case 2:
        Set_b62a2(timeline, "attackpreparation_lab");
        break;
    case 3:
        Set_b62a2(timeline, "attackpreparation_forest");
        break;
    case 4:
        Set_b62a2(timeline, "attackpreparation_sanc");
        break;
    }
}
