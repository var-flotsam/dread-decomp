// CChozoCommanderKiGrabAttack's unit (652): sends the hash for a kind (as fn_710043272c and
// fn_710043251c pick it) under mode 10 first to the owner's character class and, when that applies, to
// the player's. Returns false when either hash is -1 or the first send is refused. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "../../../src/game_s2/UnkSpitSend.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

extern const u64 g_unk_71016494b8[] __attribute__((visibility("hidden")));
extern const u64 g_unk_7101649478[] __attribute__((visibility("hidden")));
struct UnkPlayer;

// Twelve bytes (copied as a whole: an 8-byte and a 4-byte move).
struct UnkKiGrabValue_b59 {
    u32 field_0x0;
    u32 field_0x4;
    f32 value;
};

struct UnkKiGrabHashOwner_b59 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkKiGrabHash_b59 {
    u8 pad_0x0[0x8];
    UnkKiGrabHashOwner_b59* owner;
    u8 pad_0x10[0x12d];
    bool field_0x13d;
};

// The hash for a kind: kinds 3 and 4 hash a Samus sequence name, the rest come from `table`.
static inline u64 KiGrabHash_l4321a0(UnkKiGrabHash_b59* self, s32 kind, const u64* table) {
    switch (kind) {
    case 3:
        if (self->field_0x13d)
            return fn_7100001570("kigrabcommandersamusseq1prep_aura", 33);
        return fn_7100001570("kigrabcommandersamusseq1prep_counter", 36);
    case 4:
        if (self->field_0x13d)
            return fn_7100001570("kigrabcommandersamusseq1_aura", 29);
        return fn_7100001570("kigrabcommandersamusseq1_counter", 32);
    default:
        return table[kind];
    }
}

bool fn_71004321a0(UnkKiGrabHash_b59* self, s32 kind, f32 value) {
    u64 owner_hash = KiGrabHash_l4321a0(self, kind, g_unk_71016494b8);
    if (owner_hash == 0xffffffffffffffff)
        return false;
    u64 player_hash = KiGrabHash_l4321a0(self, kind, g_unk_7101649478);
    if (player_hash == 0xffffffffffffffff)
        return false;

    void* owner_target = self->owner->field_0x220;
    void* player_target =
        *reinterpret_cast<void**>(reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310)) + 0x220);
    UnkKiGrabValue_b59 param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkKiGrabValue_b59&>(g_unk_7172769740);
    }

    const u64 track = 0xf4061ff25a2163d6;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 10;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    if (!fn_7101176690(owner_target, owner_hash, track, &mode, &g_unk_710164b968, &param, &mask))
        return false;
    base::reflection::UnkReflTypeIdArg mode2;
    mode2.id = 10;
    base::reflection::UnkReflTypeIdArg mask2;
    mask2.id = UnkAll_SpitSend().value;
    return fn_7101176690(player_target, player_hash, track, &mode2, &g_unk_710164b968, &param, &mask2);
}
