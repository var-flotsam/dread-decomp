// behaviortree::CShelmitChargeAction (tools/tu_map.py): its update. Stage 1 sends the owner's (+0x18)
// animation component (+0x220) the charge track (or starts it with fn_710117ae38) and moves to stage 2; stage 3
// counts the timer at +0x54 down by the time step and, when it runs out, sends the next track and moves to
// stage 4; stage 5 is done (5), an unknown stage fails (6), and any other runs on (2) unless the animation
// component no longer plays a CCharClassShelmitChargeTrack. Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkShelmitAI_b35;
void CShelmitAIComponent_StartCrystalEmissiveRampToMin(UnkShelmitAI_b35* self, f32 dt);

struct UnkShelmitChargeCharClass_l6b7508 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x63c - 0x8];
    f32 field_0x63c;
};

struct UnkShelmitChargeAI_l6b7508 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1610 - 0x8];
    base::reflection::UnkReflObject* charClass;
};

struct UnkShelmitChargeOwner_l6b7508 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x240 - 0x228];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkShelmitChargeAction_l6b7508 {
    u8 pad_0x0[0x18];
    UnkShelmitChargeOwner_l6b7508* owner;
    u8 pad_0x20[0x30];
    s32 stage;  // 0x50
    f32 timer;  // 0x54
};

u32 fn_71006b7508(UnkShelmitChargeAction_l6b7508* self, f32 dt) {
    UnkShelmitChargeCharClass_l6b7508* charClass = nullptr;
    UnkAnimationComponent* animation = nullptr;
    UnkShelmitChargeAI_l6b7508* ai = nullptr;
    bool isShelmit = false;
    if (UnkShelmitChargeOwner_l6b7508* owner = self->owner) {
        animation = owner->animation;
        base::reflection::UnkReflObject* component = owner->field_0x240;
        if (component) {
            isShelmit = fn_710009d50c(component->vf_0x0(), fn_71006b98c0());
            if (isShelmit) {
                ai = static_cast<UnkShelmitChargeAI_l6b7508*>(component);
                base::reflection::UnkReflObject* cc = ai->charClass;
                if (cc && fn_710009d50c(cc->vf_0x0(), fn_71006b8744()))
                    charClass = static_cast<UnkShelmitChargeCharClass_l6b7508*>(cc);
            }
        }
    }

    u32 result = 2;
    switch (self->stage) {
    case 0:
    case 2:
    case 4:
        break;
    case 1:
        // Declared ahead of later case labels (no lifetime markers): each request keeps its own slot.
        base::reflection::UnkReflTypeIdArg mode, mask;
        if (!animation)
            return 2;
        mode.id = 0;
        u64 track;
        mask.id = UnkAll_SpitSend().value;
        track = 0x884c8e254ba39908;
        if (fn_7101176690(animation, track, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                          &mask) ||
            fn_710117ae38(animation, track, false)) {
            self->stage = 2;
            if (isShelmit && charClass)
                CShelmitAIComponent_StartCrystalEmissiveRampToMin(reinterpret_cast<UnkShelmitAI_b35*>(ai), charClass->field_0x63c);
        }
        break;
    case 3:
        base::reflection::UnkReflTypeIdArg mode2, mask2;
        if (self->timer >= 0.001f) {
            self->timer -= dt;
            if (self->timer < 0.001f) {
                self->timer = -1.0f;
                mode2.id = 0;
                mask2.id = UnkAll_SpitSend().value;
                if (fn_7101176690(animation, 0x6836cdff1d937e28, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958,
                                  &g_unk_7172769740, &mask2))
                    self->stage = 4;
            }
        }
        break;
    case 5:
        result = 5;
        break;
    default:
        result = 6;
        break;
    }

    if (animation && !fn_710117b348(animation, fn_7100c4e578()))
        result = 6;
    return result;
}
