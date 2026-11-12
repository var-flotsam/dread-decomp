// behaviortree::CIgnorePlayerCondition's unit (tools/tu_map.py): whether the player is ignored, from a
// tunable's mode (+0x24 of g_unk_71727bc558): mode 1 always, mode 2 while the player's (fn_7100dec788)
// +0x200 object has its flag at +0x7e4 set, otherwise never. Names and layouts are placeholders.
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTUProperty.h"
#include "types.h"

struct UnkPlayer;

struct UnkIgnorePlayerTunable_ovb0037 {
    u8 pad_0x0[0x24];
    s32 mode;  // 0x24
};

struct UnkIgnorePlayerState_ovb0037 {
    u8 pad_0x0[0x7e4];
    bool field_0x7e4;
};

struct UnkIgnorePlayer_ovb0037 {
    u8 pad_0x0[0x200];
    UnkIgnorePlayerState_ovb0037* field_0x200;
};

bool fn_710039cc98() {
    switch (reinterpret_cast<UnkIgnorePlayerTunable_ovb0037*>(&g_unk_71727bc558)->mode) {
    case 0:
        return false;
    case 1:
        return true;
    case 2: {
        auto* player = reinterpret_cast<UnkIgnorePlayer_ovb0037*>(fn_7100dec788(g_unk_7101d47310));
        if (player->field_0x200->field_0x7e4)
            return true;
        break;
    }
    }
    return false;
}
