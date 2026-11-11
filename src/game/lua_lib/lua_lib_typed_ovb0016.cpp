// Unit 2618 (tools/tu_map.py, "lua_lib"): helpers and metamethods of the typed values the Lua scripts see
// (full userdata with the "__TypedValueType" metatable holding a reflected value, base/reflection/UnkReflValue.h).
// fn_71000a1ed4 compares two values, fn_7100fbd670 and fn_7100fc0a80 read and write a member by name, and
// fn_71010a4740 is the userdata test (luaL_checkudata without the error). Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStrIdInline;
using base::reflection::UnkReflValue;

struct UnkFormatArgs;
struct UnkTextBuffer200;
struct UnkCompared_0x18;
// The userdata at `index` when its metatable is the registry's `name`, else null.
void* fn_71010a4740(lua_State* L, int index, const char* name);

// The unit's small stack visitor (vtable 0x7101cab628, address point 0x7101cab638), as in
// src/script/lua_lib_PushTyped_A6_b72.cpp.

struct UnkLuaVisitor_ovb0016 {
    void** vtable;
    u32 kind;
    void* state;
    s32 arg0;
    s32 arg1;
    s32 result;
};

// fn_7100fbde70 without the type refinement: the value is walked as given.
s32 fn_7100fbde20(void* state, UnkFormatArgs* value, s32 arg0, s32 arg1) {
    UnkLuaVisitor_ovb0016 v;
    v.kind = 1;
    v.vtable = g_unk_7101cab638;
    v.state = state;
    v.arg0 = arg0;
    v.arg1 = arg1;
    v.result = 0;
    fn_71000a1784(value, reinterpret_cast<UnkTextBuffer200*>(&v));
    return v.result;
}

// __eq: whether two typed values are equal.
int fn_7100fc0710(lua_State* L) {
    UnkReflValue* a = nullptr;
    if (lua_type(L, 1) == LUA_TUSERDATA) {
        a = static_cast<UnkReflValue*>(fn_71010a4740(L, 1, "__TypedValueType"));
        if (!a)
            luaL_typerror(L, 1, "__TypedValueType");
    }
    UnkReflValue* b = nullptr;
    if (lua_type(L, 2) == LUA_TUSERDATA) {
        b = static_cast<UnkReflValue*>(fn_71010a4740(L, 2, "__TypedValueType"));
        if (!b)
            luaL_typerror(L, 2, "__TypedValueType");
    }
    if (!a->type || !b->type) {
        luaL_error(L, "TypedObject has been deleted!");
        return 0;
    }
    lua_pushboolean(L, fn_71000a1ed4(reinterpret_cast<UnkCompared_0x18*>(a), reinterpret_cast<UnkCompared_0x18*>(b)));
    return 1;
}

struct UnkTypedPair_ovb0016 {
    char* type;
    void* obj;
};

int fn_7100fbd670(lua_State* L, UnkTypedPair_ovb0016* value, CStrId* name);
void fn_7100fc0a80(lua_State* L, UnkTypedPair_ovb0016* value, CStrId* name);

// Where the value lives: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValuePtr(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8)
        return v->field_0x0;
    return v;
}

// __index: the member named by the key.
int fn_7100fc0950(lua_State* L) {
    if (lua_type(L, 1) != LUA_TUSERDATA)
        return 0;
    UnkReflValue* v = static_cast<UnkReflValue*>(fn_71010a4740(L, 1, "__TypedValueType"));
    if (!v) {
        luaL_typerror(L, 1, "__TypedValueType");
        return 0;
    }
    if (!v->type)
        return 0;
    CStrId name(luaL_checkstring(L, 2), true);
    void* obj = ValuePtr(v);
    UnkTypedPair_ovb0016 pair = {fn_710009dcd8(const_cast<char*>(v->type), obj), obj};
    return fn_7100fbd670(L, &pair, &name);
}

// __newindex: sets the member named by the key.
int fn_7100fc0810(lua_State* L) {
    if (lua_type(L, 1) != LUA_TUSERDATA)
        return 0;
    UnkReflValue* v = static_cast<UnkReflValue*>(fn_71010a4740(L, 1, "__TypedValueType"));
    if (!v) {
        luaL_typerror(L, 1, "__TypedValueType");
        return 0;
    }
    if (!v->type) {
        luaL_error(L, "luaTypedValue_settable: TypedValue has been deleted!");
        return 0;
    }
    CStrId name(luaL_checkstring(L, 2), true);
    void* obj = ValuePtr(v);
    UnkTypedPair_ovb0016 pair = {fn_710009dcd8(const_cast<char*>(v->type), obj), obj};
    fn_7100fc0a80(L, &pair, &name);
    return 0;
}

// Reads a file path id argument (the CFilePathStrId counterpart of fn_7100fbfd20).
bool fn_7100fc01d0(lua_State* L, CFilePathStrId* out, int index) {
    if (!lua_isstring(L, index))
        return false;
    CFilePathStrId id(UnkStrIdInline(), luaL_checkstring(L, index), true);
    *out = static_cast<CFilePathStrId&&>(id);
    return true;
}
