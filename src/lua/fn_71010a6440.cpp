// The game's replacement for Lua's `print`: the same loop as luaB_print, but output goes
// through a function pointer installed at startup instead of fputs.
#include <stdio.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"

extern int (*g_puts_71010a6440)(const char*, FILE*);

int fn_71010a6440(lua_State* L) {
    int n = lua_gettop(L);
    int i;
    lua_getglobal(L, "tostring");
    for (i = 1; i <= n; i++) {
        const char* s;
        lua_pushvalue(L, -1);
        lua_pushvalue(L, i);
        lua_call(L, 1, 1);
        s = lua_tostring(L, -1);
        if (s == NULL)
            return luaL_error(L, LUA_QL("tostring") " must return a string to " LUA_QL("print"));
        if (i > 1) g_puts_71010a6440("\t", stdout);
        g_puts_71010a6440(s, stdout);
        lua_pop(L, 1);
    }
    g_puts_71010a6440("\n", stdout);
    return 0;
}
