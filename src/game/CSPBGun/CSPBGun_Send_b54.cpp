// CSPBGun's unit (tools/tu_map.py): unless the owner's (+0x18) sub-object at +0x220 is missing or
// fn_710117b348 accepts CSPBGun's class getter fn_7100bf36f8 for it, sends it a fixed hash with
// mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h) and returns what that returns. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkSPBGunOwner_b54 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkSPBGun_b54 {
    u8 pad_0x0[0x18];
    UnkSPBGunOwner_b54* owner;
};

bool fn_710085e7e8(UnkSPBGun_b54* self) {
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target || fn_710117b348(target, fn_7100bf36f8()))
        return false;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, 0x3b394ca36bf0c209, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                         &g_unk_7172769740, &mask);
}
