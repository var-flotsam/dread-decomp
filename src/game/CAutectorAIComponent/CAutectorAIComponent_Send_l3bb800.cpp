// CAutectorAIComponent's unit: sends the owner's animation component (+0x220 of +0x30) a mode-0 request
// under one of two ids, by the state at +0x146f8 (as UnkSpitSend does). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkAutectorOwner_l3bb800 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkAutectorAI_l3bb800 {
    u8 pad_0x0[0x30];
    UnkAutectorOwner_l3bb800* owner;
    u8 pad_0x38[0x146f8 - 0x38];
    u32 state;  // 0x146f8
};

static inline void Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

void fn_71003bb800(UnkAutectorAI_l3bb800* self) {
    if (void* animation = self->owner->animation) {
        switch (self->state) {
        case 1:
            base::reflection::UnkReflTypeIdArg mode_a;
            base::reflection::UnkReflTypeIdArg mask_a;
            Send(animation, 0x78e9adc802559c36ull, &mode_a, &mask_a);
            break;
        default:
            base::reflection::UnkReflTypeIdArg mode_b;
            base::reflection::UnkReflTypeIdArg mask_b;
            Send(animation, 0x1cdf12aa84de383cull, &mode_b, &mask_b);
            break;
        }
    }
}
