// CNailongAIComponent's unit: a message handler. After the behavior-tree base's (fn_710036974c): with the
// message's flag set and the owner's +0x278 object answering, marks the state, sets the model's colour (white
// in state 2, else black) and turns one animation switch on; with it clear, once the mark is cleared sends the
// class a mode-10 request and turns the switch off. Returns the base's answer. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/math/CVector4D.h"
#include "game/CAnimationComponent.h"
#include "game/CAttackComponent.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/UnkTU127.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::math::CVector4D;

struct UnkNailongKey_l621e8c;
__attribute__((visibility("hidden"))) extern UnkNailongKey_l621e8c g_unk_71726f7e58;

struct UnkNailongModelPart_l621e8c {
    u8 pad_0x0[0x138];
    void* field_0x138;
};

struct UnkNailongModel_l621e8c {
    u8 pad_0x0[0x50];
    UnkNailongModelPart_l621e8c* field_0x50;
};

struct UnkNailongOwner_l621e8c {
    u8 pad_0x0[0x220];
    void* field_0x220;
    UnkNailongModel_l621e8c* field_0x228;
    u8 pad_0x230[0x278 - 0x230];
    void* field_0x278;
};

struct UnkNailongState_l621e8c {
    s32 field_0x0;
    u8 pad_0x4[0xc];
    u8 field_0x10[1];  // a byte array: its stores may touch any object
};

struct UnkNailongAI_l621e8c {
    u8 pad_0x0[0x30];
    UnkNailongOwner_l621e8c* owner;
    u8 pad_0x38[0x148c8 - 0x38];
    UnkNailongState_l621e8c state;
};
static_assert(__builtin_offsetof(UnkNailongAI_l621e8c, state) == 0x148c8, "layout");

struct UnkNailongMessage_l621e8c {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

u32 fn_7100621e8c(UnkNailongAI_l621e8c* self, UnkNailongMessage_l621e8c* msg) {
    UnkNailongState_l621e8c* state = &self->state;
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self),
                               reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    void* animation = self->owner->field_0x220;
    if (msg->field_0x8) {
        void* part = self->owner->field_0x278;
        if (part && fn_7101136308(part)) {
            state->field_0x10[0] = 1;
            switch (state->field_0x0) {
            case 2:
                // One colour for both cases, declared before the later label.
                CVector4D colour;
                colour.x = 1.0f;
                colour.y = 1.0f;
                colour.z = 1.0f;
                colour.w = 1.0f;
                fn_71001c287c(self->owner->field_0x228->field_0x50->field_0x138, 0, 1, &colour, 1);
                colour.w = 0.0f;
                fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(self->owner->field_0x228->field_0x50), 1,
                              &colour, 1);
                break;
            default:
                colour.x = 0.0f;
                colour.y = 0.0f;
                colour.z = 0.0f;
                colour.w = 1.0f;
                fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(self->owner->field_0x228->field_0x50), 0,
                              &colour, 1);
                break;
            }
        }
        if (animation)
            fn_710117abfc(animation, true, &g_unk_71726f7e58, 0xa8067db9aac6a143);
    } else {
        if (state->field_0x10[0]) {
            state->field_0x10[0] = 0;
            if (void* target = self->owner->field_0x220) {
                base::reflection::UnkReflTypeIdArg mode;
                mode.id = 10;
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7101176690(target, 0x6a83a50c8f7f1ee6, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask);
            }
        }
        fn_710117abfc(animation, false, &g_unk_71726f7e58, 0xa8067db9aac6a143);
    }
    return result;
}
