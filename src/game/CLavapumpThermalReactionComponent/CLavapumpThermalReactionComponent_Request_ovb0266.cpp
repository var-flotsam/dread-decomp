// CLavapumpThermalReactionComponent's request: sends the owner's animation component (+0x220 of +0x30) a
// mode-0 request under one of two ids (as UnkSpitSend does), by the flag. Returns 0. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkLavapumpOwner_b0266 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkLavapump_b0266 {
    u8 pad_0x0[0x30];
    UnkLavapumpOwner_b0266* owner;
};

static inline void Send(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg* mode,
                        base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 0;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(animation, hash, 0xf4061ff25a2163d6ull, mode, &g_unk_710164b958, &g_unk_7172769740, mask);
}

u32 fn_7100a29a68(UnkLavapump_b0266* self, bool flag) {
    if (void* animation = self->owner->animation) {
        switch (static_cast<int>(flag)) {
        case 1:
            base::reflection::UnkReflTypeIdArg mode_a;
            base::reflection::UnkReflTypeIdArg mask_a;
            Send(animation, 0xe2ea001660c8ac66ull, &mode_a, &mask_a);
            break;
        default:
            base::reflection::UnkReflTypeIdArg mode_b;
            base::reflection::UnkReflTypeIdArg mask_b;
            Send(animation, 0x1432c55a4f99ab04ull, &mode_b, &mask_b);
            break;
        }
    }
    return 0;
}
