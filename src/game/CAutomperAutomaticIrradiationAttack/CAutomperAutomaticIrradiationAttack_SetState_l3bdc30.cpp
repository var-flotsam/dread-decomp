// CAutomperAutomaticIrradiationAttack's unit (unit 564): sets the attack's state (+0x30) and, for states 1, 3
// and 4, sends the owner's (+0x8) character class a zero under that state's track (as
// src/game_s2/UnkSpitSend.h). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkAutomperIrradiation_l3bdc30 {
    void** vtable;
    UnkActor* owner;
    u8 pad_0x10[0x20];
    s32 state;  // 0x30
};

void fn_71003bdc30(UnkAutomperIrradiation_l3bdc30* self, s32 state) {
    self->state = state;
    void* cls = self->owner->field_0x220;
    switch (state) {
    case 1:
        base::reflection::UnkReflTypeIdArg mode1;
        mode1.id = 0;
        base::reflection::UnkReflTypeIdArg mask1;
        mask1.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(cls, 0x3060aaa4f4b526e4, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                      &mask1);
        break;
    case 3:
        base::reflection::UnkReflTypeIdArg mode3;
        mode3.id = 0;
        base::reflection::UnkReflTypeIdArg mask3;
        mask3.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(cls, 0xcb24643fce5b5852, 0xf4061ff25a2163d6, &mode3, &g_unk_710164b958, &g_unk_7172769740,
                      &mask3);
        break;
    case 4:
        base::reflection::UnkReflTypeIdArg mode4;
        mode4.id = 0;
        base::reflection::UnkReflTypeIdArg mask4;
        mask4.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(cls, 0x56ad73034f8749b0, 0xf4061ff25a2163d6, &mode4, &g_unk_710164b958, &g_unk_7172769740,
                      &mask4);
        break;
    }
}
