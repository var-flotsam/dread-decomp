// CChozoCommanderKiGrabAttack's unit (652): sends the hash for a kind (as fn_710043272c and
// fn_710043251c pick it) under mode 10 first to the owner's character class and, when that applies, to
// the player's. Returns false when either hash is -1 or the first send is refused. Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "game/CChozoCommanderKiGrabAttack.h"
#include "game/UnkGameManager.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU1730.h"
#include "game/UnkKiGrabAttack.h"
#include "game/UnkTU2553.h"
#include "types.h"

bool fn_71004321a0(UnkKiGrabAttack* self, s32 kind, f32 value) {
    u64 owner_hash = KiGrabHash(self, kind, g_unk_71016494b8);
    if (owner_hash == 0xffffffffffffffff)
        return false;
    u64 player_hash = KiGrabHash(self, kind, g_unk_7101649478);
    if (player_hash == 0xffffffffffffffff)
        return false;

    void* owner_target = self->owner->field_0x220;
    void* player_target =
        *reinterpret_cast<void**>(reinterpret_cast<char*>(fn_7100dec788(g_unk_7101d47310)) + 0x220);
    UnkKiGrabValue param;
    if (value != -1.0f) {
        param.field_0x0 = 0;
        param.field_0x4 = 0;
        param.value = value;
    } else {
        param = reinterpret_cast<const UnkKiGrabValue&>(g_unk_7172769740);
    }

    const u64 track = 0xf4061ff25a2163d6;
    base::reflection::UnkReflTypeIdArg mode;
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
