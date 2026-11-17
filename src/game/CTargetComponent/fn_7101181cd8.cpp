// CTargetComponent's unit (tools/tu_map.py): an update. After the base update, keeps a weak handle (+0x40)
// to the current player unless the one it holds already points at it. Names and layouts are placeholders.
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTargetComponent_l181cd8 {
    u8 pad_0x0[0x40];
    UnkWeakHandle* player;  // 0x40
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7101181cd8(UnkTargetComponent_l181cd8* self) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (!player)
        return;
    if (!self->player || reinterpret_cast<UnkPlayer*>(self->player->target) != player)
        AssignRef(&self->player, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(player)));
}
