// The game's additions to Lua's auxiliary library (they sit among the lauxlib functions of the
// embedded Lua, tools/tu_map.py unit 2676): a boolean argument check (fn_71010a4900), running a
// buffer as a chunk with the error printed through a hook (fn_71010a58e0), and the hook's default,
// a printf to stderr (fn_71010a5a70).
#include <stdarg.h>
#include <stdio.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"

// The error printer the game may replace (reached through the GOT); fn_71010a5a70 until then.
extern int (*g_unk_7101caf838)(const char* fmt, ...);

// lauxlib's static reader for luaL_loadbuffer (getS) and its state.
struct UnkLuaLoadS {
    const char* s;
    size_t size;
};
const char* fn_71010a57d0(lua_State* L, void* ud, size_t* size) __attribute__((visibility("hidden")));

#if !defined(__aarch64__)
// The port: lib/lua keeps getS static, so the reader is defined here: the whole buffer once, then
// the end of the chunk.
const char* fn_71010a57d0(lua_State* L, void* ud, size_t* size) {
    (void)L;
    UnkLuaLoadS* ls = static_cast<UnkLuaLoadS*>(ud);
    if (ls->size == 0)
        return NULL;
    *size = ls->size;
    ls->size = 0;
    return ls->s;
}
#endif

// The boolean at narg: false only for a false boolean, any other type an argument error (as
// luaL_typerror, inlined).
int fn_71010a4900(lua_State* L, int narg) {
    int b = lua_toboolean(L, narg);
    if (b == 0 && lua_type(L, narg) != LUA_TBOOLEAN)
        luaL_argerror(L, narg,
                      lua_pushfstring(L, "%s expected, got %s", lua_typename(L, LUA_TBOOLEAN), luaL_typename(L, narg)));
    return b;
}

// Loads the buffer as a chunk (luaL_loadbuffer, inlined) and runs it; on an error the message
// (when there is one) goes to the error printer and is popped. Returns the load or call status.
int fn_71010a58e0(lua_State* L, const char* buff, size_t size, const char* name) {
    UnkLuaLoadS ls;
    ls.s = buff;
    ls.size = size;
    int status = lua_load(L, fn_71010a57d0, &ls, name);
    if (status == 0) {
        status = lua_pcall(L, 0, LUA_MULTRET, 0);
        if (status == 0)
            return 0;
    }
    if (!lua_isnil(L, -1)) {
        const char* msg = lua_tostring(L, -1);
        if (msg == NULL)
            msg = "(error object is not a string)";
        g_unk_7101caf838("%s\n", msg);
        lua_pop(L, 1);
    }
    return status;
}

// The error printer's default: printf to stderr.
int fn_71010a5a70(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int n = vfprintf(stderr, fmt, args);
    va_end(args);
    return n;
}
