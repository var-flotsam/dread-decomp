// CChozoCommanderKiGrabAttack's unit (tools/tu_map.py): sends the owner's (+0x8) character class (+0x220)
// the hash for a kind: kinds 3 and 4 hash one of the Samus sequence names (aura with the flag at +0x13d
// set, else counter), the rest come from a table; -1 sends nothing and returns false. The request carries
// the value, or the default block (g_unk_7172769740) for -1, with first descriptor g_unk_710164b968.
// Returns whether it applied. fn_710043251c is the same with its own table, sending to the player's
// character class (fn_7100dec788 of the game manager). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU2553.h"
#include "types.h"

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT
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
static inline u64 KiGrabHash_b59(UnkKiGrabHash_b59* self, s32 kind, const u64* table) {
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

static inline bool KiGrabSend_b59(void* target, u64 hash, f32 value) {
    base::reflection::UnkReflTypeIdArg mode;
    UnkKiGrabValue_b59 param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkKiGrabValue_b59&>(g_unk_7172769740);
    }
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &param, &mask);
}

bool fn_710043272c(UnkKiGrabHash_b59* self, s32 kind, f32 value) {
    u64 hash = KiGrabHash_b59(self, kind, g_unk_71016494b8);
    if (hash == 0xffffffffffffffff)
        return false;
    return KiGrabSend_b59(self->owner->field_0x220, hash, value);
}

bool fn_710043251c(UnkKiGrabHash_b59* self, s32 kind, f32 value) {
    u64 hash = KiGrabHash_b59(self, kind, g_unk_7101649478);
    if (hash == 0xffffffffffffffff)
        return false;
    void* target = *reinterpret_cast<void**>(reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310)) + 0x220);
    return KiGrabSend_b59(target, hash, value);
}
