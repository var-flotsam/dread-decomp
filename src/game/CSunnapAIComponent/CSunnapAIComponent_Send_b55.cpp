// CSunnapAIComponent unit (tools/tu_map.py, the name from reflection); copy of fn_710082bec4
// (src/game/CActionSwitcherOnPullGrapplePointComponent/CGrapplePointSend_b54.cpp): runs the AI
// component's fn_7101105c94, then sends the owner's (+0x30) sub-object at +0x220 a fixed hash with
// mode 0 (UnkSpitSend, src/game_s2/UnkSpitSend.h). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkWarLotusAI;

struct UnkSunnapOwner_b55 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkSunnapAI_b55 {
    u8 pad_0x0[0x30];
    UnkSunnapOwner_b55* owner;
};

void fn_71006cba4c(UnkSunnapAI_b55* self) {
    fn_7101105c94(reinterpret_cast<UnkWarLotusAI*>(self));
    UnkSpitSend(self->owner->field_0x220, 0x3d464a66bf139712, 0xf4061ff25a2163d6);
}
