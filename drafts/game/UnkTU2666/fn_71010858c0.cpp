// Unit 2666 (Lua bindings): pushes the value of property `name` (argument 3) in section `section`
// (argument 2) of the blackboard of the entity named by argument 1, when it is a bool, float, int
// or string. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU500.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

s32 fn_71010475d0(CBlackboard* self, const CStrId& key, const CStrId& name);
bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);
bool fn_7100395c38(CBlackboard::CSection* section, const void* name, CRntString* out);

struct UnkBlackboardOwner_l0858c0 {
    u8 pad_0x0[0x1260];
    CBlackboard blackboard;  // 0x1260
};

// The entity as this function sees it.
struct UnkEntity_l0858c0 {
    u8 pad_0x0[0x240];
    UnkBlackboardOwner_l0858c0* owner;  // 0x240
};

// The entity with that name.
inline UnkWeakTarget* FindEntity(UnkGameManager* manager, const CStrId& name) {
    u64 key;
    alignas(8) u32 kind;
    u32 kind_value = g_unk_71727bebb8;
    key = name.data->Key();
    kind = kind_value;
    return fn_7100debbc8(manager, &key, &kind);
}

s32 fn_71010858c0(lua_State* L) {
    const char* entity_name = luaL_checklstring(L, 1, nullptr);
    UnkWeakTarget* target = FindEntity(g_unk_7101d47310, CStrId(entity_name, true));
    if (!target)
        return 0;
    if (UnkBlackboardOwner_l0858c0* owner = reinterpret_cast<UnkEntity_l0858c0*>(target)->owner) {
        CBlackboard* blackboard = &owner->blackboard;
        CStrId section(luaL_checklstring(L, 2, nullptr), true);
        CStrId name(luaL_checklstring(L, 3, nullptr), true);
        if (fn_71010474f4(blackboard, section, name)) {
            switch (fn_71010475d0(blackboard, section, name)) {
            case 'b': {
                bool value = false;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                    fn_710006d11c(s, &name, &value);
                return fn_71010c0b40(L, "b", value);
            }
            case 'f': {
                f32 value = 0.0f;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                    fn_7100395b34(s, &name, &value);
                return fn_71010c0b40(L, "f", (f64)value);
            }
            case 'i': {
                s32 value = 0;
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                    fn_7100395a30(s, &name, &value);
                return fn_71010c0b40(L, "i", value);
            }
            case 's': {
                UnkBufferString<0x100, g_unk_7101a194b8> value(g_unk_710162e2ed);
                if (CBlackboard::CSection* s = fn_7101047270(blackboard, section))
                    fn_7100395c38(s, &name, value.Str());
                const char* text = value.Str()->data;
                return fn_71010c0b40(L, "s", text ? text : g_unk_710162e2ed);
            }
            }
        }
    }
    return 0;
}
