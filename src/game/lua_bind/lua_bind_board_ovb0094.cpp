// lua_bind's unit (unit 2674 of tools/tu_map.py): script bindings over the blackboard embedded in the
// game manager at +0x820: whether it has a section (fn_710109f3e0) or a section's name (fn_710109f4b0),
// as src/game/lua_bind/lua_bind_blackboard_ovb0073.cpp's do for +0x968. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);

// The blackboard at +0x820 of the game manager.
static inline CBlackboard* Blackboard_ovb0094(UnkGameManager* manager) {
    return reinterpret_cast<CBlackboard*>(reinterpret_cast<u8*>(manager) + 0x820);
}

// Whether the blackboard has the section named by the argument.
int fn_710109f3e0(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* name;
    bool has = false;
    if (fn_71010c0270(L, "s", &name))
        has = fn_7101047208(Blackboard_ovb0094(manager), CStrId(name, true));
    return fn_71010c0b40(L, "b", has);
}

// A string id handle (fn_7100ae986c) on the stack, its id released when it goes.
struct UnkLuaIdHandle_ovb0094 {
    union {
        CStrId id;
    };
    void* field_0x8;

    explicit UnkLuaIdHandle_ovb0094(const CStrId& from) { fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), from); }
    ~UnkLuaIdHandle_ovb0094() { id.~CStrId(); }
};

// Whether the blackboard's section (first argument) has the name (second argument).
int fn_710109f4b0(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section;
    const char* name;
    bool has = false;
    if (fn_71010c0270(L, "ss", &section, &name)) {
        CBlackboard* blackboard = Blackboard_ovb0094(manager);
        CStrId name_id(name, true);
        UnkLuaIdHandle_ovb0094 handle(name_id);
        has = fn_71010474f4(blackboard, CStrId(section, true), handle.id);
    }
    return fn_71010c0b40(L, "b", has);
}
