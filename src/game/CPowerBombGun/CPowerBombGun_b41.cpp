// CPowerBombGun unit. fn_71008566f8: when the gun has nothing left to fire (vtable slot 0x1e0 gives
// at most zero) and the player's inventory lacks the line bomb (ITEM_WEAPON_LINE_BOMB), gives the
// actor the timeline key g_unk_710164ad30 and calls "OnBombEmptyPulse" on the in-game menu's HUD
// (its vtable slot 0x1e8). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkInventory;
void* fn_7100df55e4(UnkGameManager*);
bool fn_71011428bc(UnkInventory* inventory, const UnkQualifiedId& item);

extern u64 g_unk_710164ad30[];

struct UnkSelf_pb41 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkActor* actor;  // 0x18
};

static inline bool Has_pb41(UnkInventory* inventory, UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return fn_71011428bc(inventory, qid);
}

void fn_71008566f8(UnkSelf_pb41* self) {
    typedef f32 (*AmountFn)(UnkSelf_pb41*);
    if (!(reinterpret_cast<AmountFn>(self->vtable[0x1e0 / 8])(self) <= 0.0f))
        return;
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    if (inventory && Has_pb41(inventory, g_unk_717275a7d8))
        return;
    fn_7100d7c828(self->actor, g_unk_710164ad30, false);
    UnkIngameMenuHud* hud = fn_7100deac84(g_unk_7101d47310)->field_0x2d0;
    if (hud) {
        typedef void (*CallFn)(UnkIngameMenuHud*, const char*, u32);
        reinterpret_cast<CallFn>((*reinterpret_cast<void***>(hud))[0x1e8 / 8])(hud, "OnBombEmptyPulse", 0);
    }
}
