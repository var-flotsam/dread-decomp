// Unit 2666 (lua_bind): with a positive value from the script and a player present, sets the
// inventory section's maximum life to it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdHandle {
    union {
        CStrId id;
    };
    void* field_0x8;
    UnkStrIdHandle() {}
    ~UnkStrIdHandle() { id.~CStrId(); }
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

s32 fn_710107bc60(lua_State* L) {
    f32 value = 0.0f;
    if (fn_71010c0270(L, "f", &value) && value > 0.0f) {
        if (fn_7100dec788(g_unk_7101d47310)) {
            UnkGameManager* manager = g_unk_7101d47310;
            CBlackboard* blackboard = &manager->field_0x968;
            UnkStrIdHandle key;
            {
                CStrId id("ITEM_MAX_LIFE", true);
                fn_7100ae986c(&key, id);
            }
            CStrId section_id("PLAYER_INVENTORY", true);
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
                if (!manager->field_0x968.field_0xe8) {
                    UnkTypedValue v;
                    v.type = fn_710000abbc();
                    v.value = &value;
                    fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&key), &v);
                }
            }
            fn_7100dffd48(g_unk_7101d47310);
        }
    }
    return 0;
}
