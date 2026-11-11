#include "game/UnkGameManager.h"
#include "types.h"

void fn_710071a398(void* target, u32* mask, f32 a, f32 b);

// A channel mask with no channels set (its constructor is not a constant expression).
struct UnkChannelMask_t {
    u32 bits;
    UnkChannelMask_t() { bits = 0; }
};

inline const UnkChannelMask_t& NoChannels_t() {
    static UnkChannelMask_t s_none;
    return s_none;
}

// Every channel (the low 11 bits).
inline u32 AllChannels_t() {
    static const u32 s_all = ~NoChannels_t().bits & 0x7ff;
    return s_all;
}

struct UnkSoundOwner_t {
    u8 pad_0x0[0x2b8];
    void* field_0x2b8;
};
struct UnkSoundSelf_t {
    u8 pad_0x0[0x30];
    UnkSoundOwner_t* owner;
    u8 pad_0x38[0x14510 - 0x38];
    bool field_0x14510;
};

void fn_7101104d58(UnkSoundSelf_t* self) {
    if (!self->field_0x14510)
        return;
    void* target = self->owner->field_0x2b8;
    if (!target)
        return;
    UnkGameManager* gm = g_unk_7101d47310;
    alignas(8) u32 mask_else;
    alignas(8) u32 mask_if;
    if (gm->field_0xc8 || gm->field_0x10c) {
        mask_if = AllChannels_t();
        fn_710071a398(target, &mask_if, 1.0f, 0.0f);
    } else {
        mask_else = AllChannels_t();
        fn_710071a398(target, &mask_else, 0.1f, 0.0f);
    }
}
