// CPlayerLifeComponent's unit (unit 2716): sets the player's life when it differs from the current one,
// refreshes the life display (fn_7101164cc0) and the whole-tank count, and stores the new life in the
// inventory's ITEM_CURRENT_LIFE. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CInventoryComponent.h"
#include "game/CPlayerLifeComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkItemProperties.h"
#include "types.h"

struct UnkInventory;
struct UnkLifeState_ovb0000;
struct UnkPlayerLife_ovb0049;

void fn_7101160278(UnkLifeState_ovb0000* self, f32 value);

struct UnkPlayerLifeActor_l165b50 {
    u8 pad_0x0[0x270];
    UnkInventory* inventory;  // 0x270
};

struct UnkPlayerLife_l165b50 {
    u8 pad_0x0[0x30];
    UnkPlayerLifeActor_l165b50* actor;  // 0x30
    u8 pad_0x38[0x1c];
    f32 life;  // 0x54
    u8 pad_0x58[0x2b4];
    f32 life_per_tank;  // 0x30c
    u8 pad_0x310[0x4];
    s32 tanks;  // 0x314
};
static_assert(__builtin_offsetof(UnkPlayerLife_l165b50, tanks) == 0x314, "layout");

static inline const UnkQualifiedId& GetQualifiedId_l165b50(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7101165b50(UnkPlayerLife_l165b50* self, f32 value, bool notify) {
    if (!(self->life >= value - 1e-5f && self->life <= value + 1e-5f)) {
        fn_7101160278(reinterpret_cast<UnkLifeState_ovb0000*>(self), value);
        fn_7101164cc0(reinterpret_cast<UnkPlayerLife_ovb0049*>(self));
        s32 tanks = (s32)(self->life / self->life_per_tank);
        self->tanks = tanks < 0 ? 0 : tanks;
        if (self->actor) {
            if (UnkInventory* inventory = self->actor->inventory)
                fn_7101142a28(inventory, GetQualifiedId_l165b50(g_unk_717275a118), true, true, true, self->life,
                              0.0f);
        }
    }
}
