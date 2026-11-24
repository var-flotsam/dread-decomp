// CHyperBeamGun's unit (tools/tu_map.py unit 1305): the gun's reset. After the base reset (fn_71008358b4) it
// clears two fields, loads its two effect objects through the overlay-model manager (created on first use) and
// switches them off, then takes every weapon and gadget item from the player's inventory (sets each to zero).
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CHyperBeamGun.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkModelManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2553.h"
#include "types.h"

struct UnkHyperBeamGun_l84607c;
void fn_71008358b4(UnkHyperBeamGun_l84607c* self);
extern const u8 g_unk_71727265a8[] __attribute__((visibility("hidden")));
extern const u8 g_unk_71727265b0[] __attribute__((visibility("hidden")));

struct UnkHyperBeamGun_l84607c {
    u8 pad_0x0[0x1890];
    void* field_0x1890;
    void* field_0x1898;
    u32 field_0x18a0;
    u8 pad_0x18a4[0x18fa - 0x18a4];
    u8 field_0x18fa;
};
static_assert(__builtin_offsetof(UnkHyperBeamGun_l84607c, field_0x18fa) == 0x18fa, "layout");

struct UnkPlayer_l84607c {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

// The overlay-model manager, created on first use.
static inline UnkModelManager* Manager() {
    UnkModelManager* m = g_unk_71726b0cd8;
    if (!m) {
        m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return m;
}

// Takes the whole item from the inventory.
static inline void Clear(UnkInventory* inventory, UnkPropertyEx<>& item) {
    UnkQualifiedId& id = ItemQualifiedId(item);
    if (id.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&id, ItemSource(item), base::global::CStrId::Empty());
    fn_71011406b4(inventory, id, 0, true, true, true, 0.0f);
}

void CHyperBeamGun_LoadSwitchObjectsAndClearInventoryItems(UnkHyperBeamGun_l84607c* self) {
    fn_71008358b4(self);
    self->field_0x18a0 = 0;
    self->field_0x18fa = 0;
    void* effect = fn_71003b7db0(Manager(), g_unk_71727265a8, false, true);
    self->field_0x1890 = effect;
    if (effect)
        fn_7100146dac(static_cast<UnkSwitchObject*>(effect), false);
    void* glow = fn_7100846678(Manager(), g_unk_71727265b0, 0, 1);
    self->field_0x1898 = glow;
    if (glow)
        fn_7100145e94(static_cast<UnkSwitchObject*>(glow), false);
    UnkPlayer_l84607c* player = reinterpret_cast<UnkPlayer_l84607c*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    UnkInventory* inventory = player->inventory;
    if (!inventory)
        return;
    Clear(inventory, g_unk_717275a1d8);
    Clear(inventory, g_unk_717275a218);
    Clear(inventory, g_unk_717275a758);
    Clear(inventory, g_unk_717275a798);
    Clear(inventory, g_unk_717275a518);
    Clear(inventory, g_unk_717275a5d8);
    Clear(inventory, g_unk_717275a558);
    Clear(inventory, g_unk_717275a6d8);
    Clear(inventory, g_unk_717275a658);
    Clear(inventory, g_unk_717275ab98);
    Clear(inventory, g_unk_717275a598);
    Clear(inventory, g_unk_717275aad8);
    Clear(inventory, g_unk_717275ab58);
    Clear(inventory, g_unk_717275aa58);
    Clear(inventory, g_unk_717275a158);
    Clear(inventory, g_unk_717275a198);
}
