// CChozoZombieXPoisonClawsAttack's unit: sends the sub-object at +0x58 a mode-2 request under the given hash,
// with one of two descriptor pairs chosen by the flag (as UnkSpitSend does). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct UnkPoisonClaws_l49e870 {
    u8 pad_0x0[0x58];
    void* field_0x58;
};

static inline void Send(void* target, u64 hash, base::reflection::UnkReflTypeIdArg* mode, const void* a,
                        const void* b, base::reflection::UnkReflTypeIdArg* mask) {
    mode->id = 2;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6ull, mode, a, b, mask);
}

void fn_710049e870(UnkPoisonClaws_l49e870* self, u64 hash, bool flag) {
    void* target = self->field_0x58;
    switch (static_cast<int>(flag)) {
    case 1:
        base::reflection::UnkReflTypeIdArg mode_a;
        base::reflection::UnkReflTypeIdArg mask_a;
        Send(target, hash, &mode_a, &g_unk_710164b968, &g_unk_7172769770, &mask_a);
        break;
    default:
        base::reflection::UnkReflTypeIdArg mode_b;
        base::reflection::UnkReflTypeIdArg mask_b;
        Send(target, hash, &mode_b, &g_unk_710164b958, &g_unk_7172769740, &mask_b);
        break;
    }
}
