// Unit 2677 (the script-call helpers next to src/base/UnkLuaCall.cpp): two variadic forms of fn_71010c02f0,
// and the test of whether "table.field" names a script function. Names and layouts are placeholders.
#include <stdarg.h>
#include <string.h>

#include "../../../lib/lua/lua.h"
#include "game/UnkTU2677.h"
#include "types.h"

s32 fn_71010c02f0(lua_State* L, s32 mode, bool flag, const char* fmt, va_list args);

s32 fn_71010c0270(lua_State* L, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s32 r = fn_71010c02f0(L, 0, false, fmt, args);
    va_end(args);
    return r;
}

s32 fn_71010c0ad0(lua_State* L, s32 mode, bool flag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s32 r = fn_71010c02f0(L, mode, flag, fmt, args);
    va_end(args);
    return r;
}

// A global table's field that holds a function: true when `str` is "table.field" and it does.
s32 fn_71010c0bb0(lua_State* L, const char* str) {
    const char* dot = strchr(str, '.');
    if (!dot)
        return 0;
    u32 tableLen = dot - str;
    u32 fieldLen = strlen(str) + ~tableLen;
    char table[256];
    char field[256];
    memcpy(table, str, tableLen);
    memcpy(field, dot + 1, fieldLen);
    table[tableLen] = 0;
    field[fieldLen] = 0;
    lua_getfield(L, LUA_GLOBALSINDEX, table);
    if (lua_type(L, -1) == LUA_TNIL) {
        lua_remove(L, -1);
        return 0;
    }
    lua_pushstring(L, field);
    lua_gettable(L, -2);
    lua_remove(L, -2);
    if (lua_type(L, -1) == LUA_TFUNCTION)
        return 1;
    lua_settop(L, -2);
    return 0;
}
