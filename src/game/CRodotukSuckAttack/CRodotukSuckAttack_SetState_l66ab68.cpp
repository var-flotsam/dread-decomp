// CRodotukSuckAttack unit: sets the attack's state. State 3 stops a pending player-info effect and runs
// the attack's reset; state 1 clears the effect flag and a counter; state 0 sends the owner's animation
// (+0x220) a request chosen by the CRodotukAIComponent's mode. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CPlayerInfo.h"
#include "game/CRodotukSuckAttack.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkRodotukAI_l66ab68 : UnkReflObject {
    u8 pad_0x8[0x14860 - 0x8];
    s32 mode;  // 0x14860
};

struct UnkSuckOwner_l66ab68 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkRodotukAI_l66ab68* ai;  // 0x240
};

struct UnkRodotukSuckAttack_l66ab68 {
    void** vtable;
    UnkSuckOwner_l66ab68* owner;  // 0x8
    u8 pad_0x10[0x60 - 0x10];
    s32 state;  // 0x60
    s32 field_0x64;
    bool effect_pending;  // 0x68
};

struct UnkPlayerInfo_ovb0071;

struct UnkRodotukSuckAttack_l66a624;

// The object when it is of the getter's class (or derives from it), else null.
template <class T>
inline T* Cast(T* object, const base::reflection::CClass* (*get_class)()) {
    if (!object)
        return nullptr;
    return fn_710009d50c(object->vf_0x0(), get_class()) ? object : nullptr;
}

static inline void Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    CAnimationComponent_SendActionToTrack(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

void fn_710066ab68(UnkRodotukSuckAttack_l66ab68* self, s32 state) {
    self->state = state;
    void* animation = self->owner->animation;
    UnkRodotukAI_l66ab68* ai_object = self->owner->ai;
    UnkRodotukAI_l66ab68* ai = Cast(ai_object, fn_7100671df8);

    switch (state) {
    case 0:
        if (!animation || !ai)
            return;
        switch (ai->mode) {
        case 3:
            base::reflection::UnkReflTypeIdArg mode_a;
            base::reflection::UnkReflTypeIdArg mask_a;
            Send(animation, 0xc0d9e01e1e648690ull, &mode_a, &mask_a);
            break;
        case 1:
            base::reflection::UnkReflTypeIdArg mode_b;
            base::reflection::UnkReflTypeIdArg mask_b;
            Send(animation, 0x0ab30c3b2aaf96f7ull, &mode_b, &mask_b);
            break;
        default:
            base::reflection::UnkReflTypeIdArg mode_c;
            base::reflection::UnkReflTypeIdArg mask_c;
            Send(animation, 0x2f6c55b04b61242cull, &mode_c, &mask_c);
            break;
        }
        break;
    case 1:
        self->effect_pending = false;
        self->field_0x64 = 0;
        break;
    case 3:
        if (self->effect_pending) {
            fn_7100f0dd3c(reinterpret_cast<UnkPlayerInfo_ovb0071*>(g_unk_7101d47310->field_0x38));
            self->effect_pending = false;
        }
        fn_710066a760(reinterpret_cast<UnkRodotukSuckAttack_l66a624*>(self));
        break;
    }
}
