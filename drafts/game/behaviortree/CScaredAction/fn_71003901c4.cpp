// behaviortree::CScaredAction's unit (tools/tu_map.py): the update step. It steps the state at +0x30
// through the scared animation (requests sent as UnkSpitSend, src/game_s2/UnkSpitSend.h), counts the
// timer at +0x34 down, and returns 2 (running), 5 (done) or 6 (track lost). Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkScaredFear_l3901c4 {
    u8 pad_0x0[0x1128];
    f32 field_0x1128;
};

struct UnkScaredOwner_l3901c4 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    UnkScaredFear_l3901c4* field_0x240;
};

struct UnkScaredAction_l3901c4 {
    u8 pad_0x0[0x18];
    UnkScaredOwner_l3901c4* owner;
    u8 pad_0x20[0xc];
    f32 field_0x2c;
    u32 state;
    f32 timer;
};

u32 fn_71003901c4(UnkScaredAction_l3901c4* self, f32 dt) {
    UnkScaredOwner_l3901c4* owner = self->owner;
    UnkAnimationComponent* animation = owner ? owner->field_0x220 : nullptr;
    UnkScaredFear_l3901c4* fear = owner ? owner->field_0x240 : nullptr;
    bool hasAnimation = animation != nullptr;
    if (hasAnimation && fn_710117b348(animation, fn_7100d2b3a8()))
        return 2;

    u32 result = 2;
    switch (self->state) {
        base::reflection::UnkReflTypeIdArg mode1;
        base::reflection::UnkReflTypeIdArg mask1;
        base::reflection::UnkReflTypeIdArg mode3;
        base::reflection::UnkReflTypeIdArg mask3;
        base::reflection::UnkReflTypeIdArg mode4;
        base::reflection::UnkReflTypeIdArg mask4;
    case 1:
        if (!hasAnimation)
            return 2;
        mode1.id = 0;
        mask1.id = UnkAll_SpitSend().value;
        if (fn_7101176690(animation, 0x52f80cb359c52bef, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958,
                          &g_unk_7172769740, &mask1))
            self->state = 2;
        break;
    case 4:
        if (fear && ((fear->field_0x1128 < 0.5f) & hasAnimation)) {
            mode4.id = 0;
            mask4.id = UnkAll_SpitSend().value;
            if (fn_7101176690(animation, 0x14d8371df8520610, 0xf4061ff25a2163d6, &mode4, &g_unk_710164b958,
                              &g_unk_7172769740, &mask4)) {
                self->state = 3;
                self->timer = self->field_0x2c;
            }
        }
        break;
    case 3:
        if (fear && fear->field_0x1128 < 0.5f)
            self->timer = self->field_0x2c;
        if (self->timer > 0.001f)
            self->timer -= dt;
        if (self->timer <= 0.001f) {
            self->timer = 0.0f;
            if (!hasAnimation)
                return 2;
            mode3.id = 0;
            mask3.id = UnkAll_SpitSend().value;
            if (fn_7101176690(animation, 0x40cef973c887bdc4, 0xf4061ff25a2163d6, &mode3, &g_unk_710164b958,
                              &g_unk_7172769740, &mask3))
                self->state = 4;
        }
        break;
    case 5:
        result = 5;
        break;
    }

    if (hasAnimation && self->state != 5 && !fn_710117b348(animation, fn_7100c49938()))
        result = 6;
    return result;
}
