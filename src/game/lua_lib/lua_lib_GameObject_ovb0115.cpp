// Unit 2617 (tools/tu_map.py, "lua_lib"): game objects as the Lua scripts see them, full userdata with the
// "__GameObjectType" metatable holding the object's weak handle (game_s2/UnkWeakHandle.h). fn_7100fbc4e0
// pushes one, fn_7100fbcbf0 is its __eq and fn_7100fbc640 a method closure that calls the reflected
// member named by its upvalue (fn_7100fbe7b0). fn_71010a4740 is the userdata test, as
// src/game/lua_lib/lua_lib_typed_ovb0016.cpp declares it. Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "game/UnkTU2676.h"
#include "game/lua_lib.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkGameObjectRef_ovb0115 {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

// __eq: whether two game objects are the same.
int fn_7100fbcbf0(lua_State* L) {
    UnkGameObjectRef_ovb0115** a = nullptr;
    if (lua_type(L, 1) == LUA_TUSERDATA) {
        a = static_cast<UnkGameObjectRef_ovb0115**>(fn_71010a4740(L, 1, "__GameObjectType"));
        if (!a)
            luaL_typerror(L, 1, "__GameObjectType");
    }
    UnkGameObjectRef_ovb0115** b = nullptr;
    if (lua_type(L, 2) == LUA_TUSERDATA) {
        b = static_cast<UnkGameObjectRef_ovb0115**>(fn_71010a4740(L, 2, "__GameObjectType"));
        if (!b)
            luaL_typerror(L, 2, "__GameObjectType");
    }
    if (!*a || !(*a)->object || !*b || !(*b)->object) {
        luaL_error(L, "GameObject has been deleted!");
        return 0;
    }
    lua_pushboolean(L, (*a)->object == (*b)->object);
    return 1;
}

// Pushes a game object as a "__GameObjectType" userdata holding its weak handle.
int fn_7100fbc4e0(lua_State* L, UnkWeakTarget* object) {
    if (!object)
        return 0;
    UnkWeakHandle** ud = static_cast<UnkWeakHandle**>(lua_newuserdata(L, sizeof(UnkWeakHandle*)));
    *ud = nullptr;
    lua_getfield(L, LUA_REGISTRYINDEX, "__GameObjectType");
    lua_setmetatable(L, -2);
    AssignRef(ud, GetWeakHandle(object));
    return 1;
}

struct UnkGameObject_ovb0115 {
    virtual char* GetClass();
};

struct UnkGameObjectTyped_ovb0115 {
    char* type;
    void* obj;
};

int fn_7100fbe7b0(lua_State* L, UnkGameObjectTyped_ovb0115* value, CStrId* name);

// A game object's method closure: the name in the first upvalue, the object in the first argument.
int fn_7100fbc640(lua_State* L) {
    const char* str = luaL_checkstring(L, lua_upvalueindex(1));
    if (lua_type(L, 1) != LUA_TUSERDATA)
        return 0;
    UnkWeakHandle** ud = static_cast<UnkWeakHandle**>(fn_71010a4740(L, 1, "__GameObjectType"));
    if (!ud) {
        luaL_typerror(L, 1, "__GameObjectType");
        return 0;
    }
    if (!*ud)
        return 0;
    UnkGameObject_ovb0115* obj = reinterpret_cast<UnkGameObject_ovb0115*>((*ud)->target);
    if (!obj)
        return 0;
    UnkGameObjectTyped_ovb0115 value = {fn_710009dcd8(obj->GetClass(), obj), obj};
    CStrId name(str, true);
    return fn_7100fbe7b0(L, &value, &name);
}
