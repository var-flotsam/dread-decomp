// Unit 2571 (tools/tu_map.py, no name hint): an item's amount, as a whole number: the item's maximum is
// read from the game manager's blackboard (the section the id table names at +0x1bd8, under the item
// definition's qualified id, built on first use) into a value left unused, then the player's amount
// of the item is taken from the inventory (fn_7101142928). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CInventoryComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

// The id table (fn_7100031894 builds it; src/game/CGameManager.cpp has the same definition).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// As src/game/CSecondaryGun/CSecondaryGun_b41.cpp and src/game/CGameManager.cpp declare them.
struct UnkInventory;
bool fn_7100395b34(CBlackboard::CSection*, const void*, f32*);

// The item's qualified id, built on first use (as src/game_s7/fn_7100fc4b34.cpp's).
static inline const UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

template <class N>
__attribute__((always_inline)) inline void GetF32(CBlackboard* set, const CStrId& section, const N& name, f32* out) {
    if (CBlackboard::CSection* s = fn_7101047270(set, section))
        fn_7100395b34(s, &name, out);
}

u32 fn_7100ec7768() {
    f32 max = 0.0f;
    GetF32(&g_unk_7101d47310->field_0x968, *reinterpret_cast<const CStrId*>(&GameManagerIdTable().pad_0x0[0x1bd8]),
           Qid(g_unk_717275a298), &max);
    UnkInventory* inventory = static_cast<UnkInventory*>(fn_7100df55e4(g_unk_7101d47310));
    return (u32)fn_7101142928(inventory, Qid(g_unk_717275a298));
}
