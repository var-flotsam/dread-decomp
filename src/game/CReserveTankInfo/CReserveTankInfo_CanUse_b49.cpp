// CReserveTankInfo (tools/tu_map.py unit 2593): whether the reserve tank can be used now: not while the game
// manager is busy (fn_7100ded930), only when both checks of the info pass (fn_7100f10960, fn_7100f10a2c)
// and the manager's word at +0x17c is clear; when its word at +0x180 is set, also only when the player's
// inventory (fn_7100df55e4) holds the Varia suit (fn_71011428bc). Names and layout are placeholders.
#include "game/CReserveTankInfo.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "types.h"

// As src/engine/HostArguments.cpp and src/game/CGun/CGun_b45.cpp declare them.
struct UnkInventory;
bool fn_7100ded930(UnkGameManager* self);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);
void* fn_7100df55e4(UnkGameManager*);

struct UnkReserveTankInfo_b49a6;

struct UnkGameManagerWords_b49a6 {
    u8 pad_0x0[0x17c];
    u32 field_0x17c;
    u32 field_0x180;
};

static inline const UnkQualifiedId& GetQualifiedId(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

bool fn_7100f10d50(UnkReserveTankInfo_b49a6* self) {
    if (fn_7100ded930(g_unk_7101d47310))
        return false;
    if (!fn_7100f10960(self) || !fn_7100f10a2c(self))
        return false;
    auto* words = reinterpret_cast<UnkGameManagerWords_b49a6*>(g_unk_7101d47310);
    if (words->field_0x17c)
        return false;
    if (!words->field_0x180)
        return true;
    auto* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (!inventory)
        return false;
    return fn_71011428bc(inventory, GetQualifiedId(g_unk_717275a498));
}
