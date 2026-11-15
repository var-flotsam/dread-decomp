// Unit 2702: an AI component sends its owner's track the hash its target holds, unless the track is an attack
// track; returns whether the track accepted it. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkSendHashHolder_l10fbcc {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkSendTrack_l10fbcc {
    u8 pad_0x0[0x2b8];
    UnkSendHashHolder_l10fbcc* field_0x2b8;
};

struct UnkSendOwner_l10fbcc {
    u8 pad_0x0[0x220];
    UnkSendTrack_l10fbcc* track;  // 0x220
};

struct UnkSendAI_l10fbcc {
    u8 pad_0x0[0x30];
    UnkSendOwner_l10fbcc* owner;  // 0x30
};

// UnkSpitSend with mode 1.
static inline bool SendMode_l10fbcc(void* target, u64 hash, u64 kind, u32 m) {
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = m;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, kind, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
}

bool fn_710110fbcc(UnkSendAI_l10fbcc* self) {
    UnkSendTrack_l10fbcc* track = self->owner->track;
    if (!track || fn_710117b348(reinterpret_cast<UnkAnimationComponent*>(track), fn_7100ceda9c()))
        return false;
    track = self->owner->track;
    if (!track)
        return false;
    return SendMode_l10fbcc(track, track->field_0x2b8->hash, 0xf4061ff25a2163d6, 1);
}
