// CRodomithonXSuckAttack unit: sets the attack's state and, when the owner's AI (+0x240) is a
// CRodomithonXAIComponent with a CCharClassRodomithonXAIComponent and the owner has an animation component
// (+0x220), takes the state's two values from the char class and sends the animation the state's request.
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU1730.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkRodomithonXCharClass_l664724 : UnkReflObject {
    u8 pad_0x8[0x630 - 0x8];
    f32 fFireActiveTime;
    f32 fRestTime;
};

struct UnkRodomithonXAI_l664724 : UnkReflObject {
    u8 pad_0x8[0x8];
    UnkRodomithonXCharClass_l664724* char_class;  // 0x10
};

struct UnkSuckOwner_l664724 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
    u8 pad_0x228[0x240 - 0x228];
    UnkRodomithonXAI_l664724* ai;  // 0x240
};

struct UnkSuckAttack_l664724 {
    u8 pad_0x0[0x8];
    UnkSuckOwner_l664724* owner;  // 0x8
    u8 pad_0x10[0x30 - 0x10];
    s32 state;  // 0x30
    f32 timer1;  // 0x34
    f32 timer0;  // 0x38
};

// The update's name for the attack (CRodomithonXSuckAttack_Update_b65a4.cpp).
struct UnkSuckAttack_b65a4;

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

void fn_7100664724(UnkSuckAttack_b65a4* attack, s32 state) {
    auto* self = reinterpret_cast<UnkSuckAttack_l664724*>(attack);
    self->state = state;
    UnkRodomithonXAI_l664724* ai = Cast(self->owner->ai, fn_7100668b24);
    if (!ai)
        return;
    UnkRodomithonXCharClass_l664724* char_class = Cast(ai->char_class, fn_7100664e84);
    void* animation = self->owner->animation;
    if (!char_class || !animation)
        return;

    switch (state) {
    case 0:
        self->timer0 = char_class->fFireActiveTime;
        self->timer1 = 0.0f;
        base::reflection::UnkReflTypeIdArg mode_a;
        base::reflection::UnkReflTypeIdArg mask_a;
        Send(animation, 0x2f6c55b04b61242cull, &mode_a, &mask_a);
        break;
    case 1:
        self->timer1 = char_class->fRestTime;
        self->timer0 = 0.0f;
        base::reflection::UnkReflTypeIdArg mode_b;
        base::reflection::UnkReflTypeIdArg mask_b;
        Send(animation, 0x17fb33c6c9619cedull, &mode_b, &mask_b);
        break;
    case 2:
        base::reflection::UnkReflTypeIdArg mode_c;
        base::reflection::UnkReflTypeIdArg mask_c;
        Send(animation, 0x4f36c2305fee37d4ull, &mode_c, &mask_c);
        break;
    }
}
