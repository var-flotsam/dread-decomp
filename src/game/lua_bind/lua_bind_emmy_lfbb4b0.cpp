// lua_bind's unit (tools/tu_map.py): a script binding that finds an entity's door-like logic object
// (fn_710055b104) and a second entity by name, and when the second one's AI component is a
// CEmmyAIComponent, hands it the first object. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkLuaStrData.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkLuaDoorTarget;

// The entity as this binding sees it: its AI component.
struct UnkLuaEntity_lfbb4b0 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

// The CEmmyAIComponent as this binding sees it: the door it is linked to.
struct UnkEmmyAI_lfbb4b0 {
    u8 pad_0x0[0x296c0];
    UnkLuaDoorTarget* door;  // 0x296c0
};

static inline void* FindObject(UnkGameManager* manager, const CStrId& id, u32 kind_value) {
    u64 hash;
    alignas(8) u32 kind;
    u32 k = kind_value;
    u64 h = reinterpret_cast<UnkLuaStrData*>(id.data)->hash;
    hash = h;
    kind = k;
    return fn_7100deba8c(manager, &hash, &kind);
}

static inline UnkWeakTarget* FindEntity(UnkGameManager* manager, const CStrId& id, u32 kind_value) {
    u64 hash;
    alignas(8) u32 kind;
    u32 k = kind_value;
    u64 h = reinterpret_cast<UnkLuaStrData*>(id.data)->hash;
    hash = h;
    kind = k;
    return fn_7100debbc8(manager, &hash, &kind);
}

namespace lua_bind {
int SetWorldGraphToEmmy(lua_State* L) {
    const char* name;
    const char* emmy_name;
    if (fn_71010c0270(L, "ss", &name, &emmy_name) && *name && *emmy_name) {
        void* object;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(name, true);
            object = FindObject(manager, id, g_unk_71727bebb8);
        }
        UnkWeakTarget* entity;
        {
            UnkGameManager* manager = g_unk_7101d47310;
            CStrId id(emmy_name, true);
            entity = FindEntity(manager, id, g_unk_71727bebb8);
        }
        if (object && entity) {
            UnkLuaDoorTarget* door = fn_710055b104(object);
            if (UnkReflObject* ai = reinterpret_cast<UnkLuaEntity_lfbb4b0*>(entity)->ai) {
                ai = fn_710009d50c(ai->vf_0x0(), fn_710053b934()) ? ai : nullptr;
                if (door && ai)
                    reinterpret_cast<UnkEmmyAI_lfbb4b0*>(ai)->door = door;
            }
        }
    }
    return 0;
}
}  // namespace lua_bind
