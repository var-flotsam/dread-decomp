// CChozoRobotSoldierCannonShotAttack's unit (tools/tu_map.py): sends the owner's (+0x8) animation (+0x220) the
// mode 2 under hash 0xc0f66394be5c6740 when that animation is playing it (fn_710117ae38), else 0 (descriptors
// as UnkSpitSend), then clears +0x38, counts the shot at +0x40 and sets the flag pair at +0x5d. Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkCannonShotSendOwner_ovb0111 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkCannonShotSend_ovb0111 {
    u8 pad_0x0[0x8];
    UnkCannonShotSendOwner_ovb0111* owner;
    u8 pad_0x10[0x28];
    u32 field_0x38;
    u8 pad_0x3c[0x4];
    s32 field_0x40;
    u8 pad_0x44[0x19];
    bool field_0x5d;
    bool field_0x5e;
};

void fn_7100466a1c(UnkCannonShotSend_ovb0111* self) {
    u32 m = fn_710117ae38(self->owner->field_0x220, 0xc0f66394be5c6740, false) ? 2 : 0;
    UnkAnimationComponent* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, 0xc0f66394be5c6740, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                  &g_unk_7172769740, &mask);
    self->field_0x38 = 0;
    self->field_0x40++;
    self->field_0x5d = true;
    self->field_0x5e = false;
}
