// CMorphBallMovement's unit (hint only): a speed scale picked by the movement's state, reduced in slow motion and
// raised while the player's speed booster ability asks for it. Names and layouts are placeholders.

#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"

f32 fn_71010d6858();

extern base::reflection::UnkTunable g_unk_71726b0fc0;

struct UnkBallMovement_l9403c4 {
    u8 pad_0x0[0x2288];
    s32 field_0x2288;
    f32 field_0x228c;
    f32 field_0x2290;
    f32 field_0x2294;
};

// The slow-motion flags as bytes of the game manager's word at +0xc4.
struct UnkSlomoFlags_l9403c4 {
    bool enabled;
    u8 unk_0xc5;
    bool paused;
};

f32 fn_71009403c4(UnkBallMovement_l9403c4* self) {
    f32 scale = 0.0f;
    switch (self->field_0x2288) {
    case 1:
        scale = self->field_0x2290;
        break;
    case 2:
    case 3:
        scale = self->field_0x228c;
        break;
    case 4:
    case 5:
        scale = self->field_0x2294;
        break;
    }
    UnkGameManager* manager = g_unk_7101d47310;
    UnkSlomoFlags_l9403c4* slomo = reinterpret_cast<UnkSlomoFlags_l9403c4*>(&manager->field_0xc4);
    if ((manager->field_0xc8 != 0 || manager->field_0x10c) && slomo->enabled && !slomo->paused)
        scale *= fn_71010d6858();
    UnkAbility* ability = fn_7100df5928(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310));
    if (ability && fn_710033cf20(ability))
        scale *= *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726b0fc0) + 0x34);
    return scale;
}
