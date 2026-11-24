// GUI::CHUD (unit 2751): when the game has a player, refreshes the missile and power bomb counters by sending
// each handler a change of its maximum (ITEM_WEAPON_MISSILE_MAX, ITEM_WEAPON_POWER_BOMB_MAX).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkHud_2950;
struct UnkHud_42cc;

// The item-change message: the changed item's id and whether ammo was added.
struct UnkItemChange_2950 {
    base::global::CStrId item;
    u8 pad_0x8[4];
    bool filled;  // 0xc
};

// The item-change message: the changed item's id and whether ammo was added.
struct UnkItemChange_42cc {
    base::global::CStrId item;
    u8 pad_0x8[4];
    bool filled;  // 0xc
};

bool CHUD_UpdateMissileCounterOnItemChange(UnkHud_2950* self, UnkItemChange_2950 change);
bool fn_71011e42cc(UnkHud_42cc* self, UnkItemChange_42cc change);

static inline UnkQualifiedId& Qid_47d0(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

bool fn_71011e47d0(UnkHud_2950* self) {
    if (fn_7100defcd4(g_unk_7101d47310)) {
        {
            CStrId id = Qid_47d0(g_unk_717275a218).id;
            CHUD_UpdateMissileCounterOnItemChange(self, {id, {}, true});
        }
        {
            CStrId id = Qid_47d0(g_unk_717275a298).id;
            fn_71011e42cc(reinterpret_cast<UnkHud_42cc*>(self), {id, {}, true});
        }
    }
    return false;
}
