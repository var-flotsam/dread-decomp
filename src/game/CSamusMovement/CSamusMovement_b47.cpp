// CSamusMovement unit (tools/tu_map.py): true while the timer at +0x2314 runs, when the game manager's
// member at +0x38 says so (fn_7100f0ca8c), or when the owner's animation is in a
// CCharClassIgnoreHangTrack. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/CPlayerInfo.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkSamusMovementOwnerB47 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSamusMovementB47 {
    u8 pad_0x0[0x30];
    UnkSamusMovementOwnerB47* field_0x30;
    u8 pad_0x38[0x2314 - 0x38];
    f32 field_0x2314;
};

bool fn_71009ebe04(UnkSamusMovementB47* self) {
    if (self->field_0x2314 > 0.0f || fn_7100f0ca8c(reinterpret_cast<char*>(g_unk_7101d47310) + 0x38))
        return true;
    return fn_710117b348(self->field_0x30->field_0x220, fn_7100d0ccb8());
}
