// CChozoCommanderUltimateGrabAttack's unit (tools/tu_map.py, unit 658): sends the hashes for a kind under
// mode 10 first to the owner's character class and, when that applies, to the player's (as fn_7100440140
// and fn_710043ffa4 pick them). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

extern const u64 g_unk_7101649600[] __attribute__((visibility("hidden")));  // a hash per kind
extern const u64 g_unk_7101649598[] __attribute__((visibility("hidden")));  // a hash per kind

// Twelve bytes (copied as a whole: an 8-byte and a 4-byte move).
struct UnkUltimateGrabValue_ovb0130 {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;
};

struct UnkUltimateGrabOwner_ovb0130 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkUltimateGrab_ovb0130 {
    u8 pad_0x0[0x8];
    UnkUltimateGrabOwner_ovb0130* owner;
    u8 pad_0x10[0x6d0 - 0x10];
    f32 field_0x6d0;
};

// The owner's hash for a kind (kind 12 has its own while +0x6d0 is positive; kinds 0, 1, 2, 5 and 10 have
// none), as fn_7100440140 picks it.
static inline bool OwnerHash(const UnkUltimateGrab_ovb0130* self, s32 kind, u64* hash) {
    if (kind == 12 && self->field_0x6d0 > 0.0f) {
        *hash = 0x71981e37466484f0;
        return true;
    }
    if ((0x427ul >> kind) & 1)
        return false;
    *hash = g_unk_7101649600[kind];
    return true;
}

// The player's hash for a kind (kinds 0 to 2 have none), as fn_710043ffa4 picks it.
static inline bool PlayerHash(const UnkUltimateGrab_ovb0130* self, s32 kind, u64* hash) {
    if (kind == 12 && self->field_0x6d0 > 0.0f) {
        *hash = 0xd5c4b35ace782264;
        return true;
    }
    if ((u32)kind < 3)
        return false;
    *hash = g_unk_7101649598[kind];
    return true;
}

bool fn_710043fd04(UnkUltimateGrab_ovb0130* self, s32 kind, f32 value) {
    u64 owner_hash;
    if (!OwnerHash(self, kind, &owner_hash))
        return false;
    u64 player_hash;
    if (!PlayerHash(self, kind, &player_hash))
        return false;

    void* owner_target = self->owner->field_0x220;
    void* player_target =
        reinterpret_cast<UnkUltimateGrabOwner_ovb0130*>(fn_7100dec788(g_unk_7101d47310))->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    UnkUltimateGrabValue_ovb0130 param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkUltimateGrabValue_ovb0130&>(g_unk_7172769740);
    }

    const u64 track = 0xf4061ff25a2163d6;
    mode.id = 10;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (!CAnimationComponent_SendActionToTrack(owner_target, owner_hash, track, &mode, &g_unk_710164b968, &param, &mask))
        return false;
    base::reflection::UnkReflTypeIdArg mode2;
    mode2.id = 10;
    base::reflection::UnkReflTypeIdArg mask2;
    mask2.id = UnkAll_SpitSend().value;
    return CAnimationComponent_SendActionToTrack(player_target, player_hash, track, &mode2, &g_unk_710164b968, &param, &mask2);
}
