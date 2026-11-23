// Unit 2666 Lua binding: refills the player's special energy. Reads ITEM_MAX_SPECIAL_ENERGY from the
// game blackboard's PLAYER_INVENTORY section and writes it to ITEM_CURRENT_SPECIAL_ENERGY (unless the
// blackboard is locked), then lets the player's component (fn_7100303054) update (fn_7100a1f8cc).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU500.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct lua_State;

struct UnkStrIdHandle {
    union {
        CStrId id;
    };
    void* field_0x8;
    UnkStrIdHandle() {}
    ~UnkStrIdHandle() { id.~CStrId(); }
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

void fn_7100a1f8cc(UnkWeakTarget* target);

s32 fn_710107a3a0(lua_State* L) {
    {
        UnkTypedValue v;
        UnkGameManager* manager = g_unk_7101d47310;
        CBlackboard* blackboard = &manager->field_0x968;
        f32 energy = 0.0f;
        UnkStrIdHandle max_key;
        {
            CStrId id("ITEM_MAX_SPECIAL_ENERGY", true);
            fn_7100ae986c(&max_key, id);
        }
        {
            CStrId section_id("PLAYER_INVENTORY", true);
            if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
                fn_7100395b34(s, &max_key, &energy);
        }
        UnkStrIdHandle current_key;
        {
            CStrId id("ITEM_CURRENT_SPECIAL_ENERGY", true);
            fn_7100ae986c(&current_key, id);
        }
        CStrId section_id("PLAYER_INVENTORY", true);
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!manager->field_0x968.field_0xe8) {
                v.type = fn_710000abbc();
                v.value = &energy;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&current_key), &v);
            }
        }
    }
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        if (UnkWeakTarget* target = fn_7100303054(player))
            fn_7100a1f8cc(target);
    return 0;
}
