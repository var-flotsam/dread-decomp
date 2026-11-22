// Unit 2665 ("lua_lib"): __index of the scripts' display objects. A name the object's reflected class
// has comes back as a closure that calls that method (fn_7101075c30, with the name as its upvalue);
// any other name raises a Lua error and gives nil. Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkLuaDisplayObject_ovb0109.h"
#include "game/UnkTU2676.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

// The display object behind the handle: its class getter and its name.
struct UnkLuaDisplayTarget_l0767a0 {
    virtual char* GetClass();

    u8 pad_0x8[0x128];
    CStrId name;  // 0x130
};

__attribute__((visibility("hidden"))) int fn_7101075c30(lua_State* L);

static inline UnkLuaDisplayObject_ovb0109* ToDisplayObject(lua_State* L, int index) {
    return static_cast<UnkLuaDisplayObject_ovb0109*>(fn_71010a4740(L, index, "__DisplayObjectType"));
}

static inline char* AsClass(char* type) {
    return type && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(
                                     reinterpret_cast<UnkLuaDisplayTarget_l0767a0*>(type)->GetClass()),
                                 reinterpret_cast<const base::reflection::CType*>(fn_7100096100()))
               ? type
               : nullptr;
}

static inline bool HasMethod(char* type, CStrId name) {
    return fn_71000978e4(AsClass(type), &name) != nullptr;
}

int fn_71010767a0(lua_State* L) {
    if (lua_type(L, 1) != LUA_TUSERDATA)
        return 0;
    UnkLuaDisplayObject_ovb0109* ud = ToDisplayObject(L, 1);
    if (!ud) {
        luaL_typerror(L, 1, "__DisplayObjectType");
        return 0;
    }
    UnkLuaDisplayTarget_l0767a0* object = reinterpret_cast<UnkLuaDisplayTarget_l0767a0*>(ud->Get());
    if (!object)
        return 0;
    CStrId key(luaL_checkstring(L, 2), true);
    unk_prop_bag::UnkPropBag bag;
    if (HasMethod(fn_710009dcd8(object->GetClass(), object), key)) {
        lua_pushstring(L, key.Text());
        lua_pushcclosure(L, fn_7101075c30, 1);
    } else {
        luaL_error(L, "DisplayObject '%s' [%s]. Property '%s' not found", object->name.Text(),
                   reinterpret_cast<base::reflection::CType*>(object->GetClass())->m_name.Text(), key.Text());
        lua_pushnil(L);
    }
    return 1;
}
