// lua_bind's unit (unit 2675 of tools/tu_map.py): the same bindings over the blackboard at +0xeb8 of the
// game manager: whether it has a section (fn_710109ff60) or a section's name (fn_71010a0030). Names and
// layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

s32 fn_71010c0270(lua_State* L, const char* format, ...);
int fn_71010c0b40(lua_State*, const char*, ...);
struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
bool fn_71010474f4(CBlackboard* self, const CStrId& section, const CStrId& name);

// The blackboard at +0xeb8 of the game manager.
static inline CBlackboard* Blackboard_ovb0094b(UnkGameManager* manager) {
    return reinterpret_cast<CBlackboard*>(reinterpret_cast<u8*>(manager) + 0xeb8);
}

// Whether the blackboard has the section named by the argument.
int fn_710109ff60(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* name;
    bool has = false;
    if (fn_71010c0270(L, "s", &name))
        has = fn_7101047208(Blackboard_ovb0094b(manager), CStrId(name, true));
    return fn_71010c0b40(L, "b", has);
}

// A string id handle (fn_7100ae986c) on the stack, its id released when it goes.
struct UnkLuaIdHandle_ovb0094b {
    union {
        CStrId id;
    };
    void* field_0x8;

    explicit UnkLuaIdHandle_ovb0094b(const CStrId& from) { fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), from); }
    ~UnkLuaIdHandle_ovb0094b() { id.~CStrId(); }
};

// Whether the blackboard's section (first argument) has the name (second argument).
int fn_71010a0030(lua_State* L) {
    UnkGameManager* manager = g_unk_7101d47310;
    const char* section;
    const char* name;
    bool has = false;
    if (fn_71010c0270(L, "ss", &section, &name)) {
        CBlackboard* blackboard = Blackboard_ovb0094b(manager);
        UnkLuaIdHandle_ovb0094b handle(CStrId(name, true));
        has = fn_71010474f4(blackboard, CStrId(section, true), handle.id);
    }
    return fn_71010c0b40(L, "b", has);
}
