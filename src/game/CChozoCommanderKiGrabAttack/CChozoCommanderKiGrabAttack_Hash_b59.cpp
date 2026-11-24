// CChozoCommanderKiGrabAttack's unit (tools/tu_map.py): sends the owner's (+0x8) character class (+0x220)
// the hash for a kind: kinds 3 and 4 hash one of the Samus sequence names (aura with the flag at +0x13d
// set, else counter), the rest come from a table; -1 sends nothing and returns false. The request carries
// the value, or the default block (g_unk_7172769740) for -1, with first descriptor g_unk_710164b968.
// Returns whether it applied. fn_710043251c is the same with its own table, sending to the player's
// character class (fn_7100dec788 of the game manager). Names are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU1730.h"
#include "game/UnkKiGrabAttack.h"
#include "game/UnkTU2553.h"
#include "types.h"

static inline bool KiGrabSend_b59(void* target, u64 hash, f32 value) {
    base::reflection::UnkReflTypeIdArg mode;
    UnkKiGrabValue param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkKiGrabValue&>(g_unk_7172769740);
    }
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    return CAnimationComponent_SendActionToTrack(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &param, &mask);
}

bool fn_710043272c(UnkKiGrabAttack* self, s32 kind, f32 value) {
    u64 hash = KiGrabHash(self, kind, g_unk_71016494b8);
    if (hash == 0xffffffffffffffff)
        return false;
    return KiGrabSend_b59(self->owner->field_0x220, hash, value);
}

bool fn_710043251c(UnkKiGrabAttack* self, s32 kind, f32 value) {
    u64 hash = KiGrabHash(self, kind, g_unk_7101649478);
    if (hash == 0xffffffffffffffff)
        return false;
    void* target = *reinterpret_cast<void**>(reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310)) + 0x220);
    return KiGrabSend_b59(target, hash, value);
}
