// The engine's script-call helpers, next to the embedded Lua: push the arguments a format string
// describes and call a script function by name.
// Holds fn_71010bf400 (push the arguments), fn_71010c0b40 (its variadic form) and fn_71010c0e70 (call,
// discarding the result).
#include <stdarg.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "base/math/CVector.h"
#include "types.h"

extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* str);
s32 fn_710109cef0(lua_State* L, s32 nargs, s32 nresults, s32 errfunc);
void fn_7100fbc4e0(lua_State* L, void* arg);
void fn_71010bf700(lua_State* L, void* table);
base::math::CVector3D* fn_71010c0ff0(lua_State* L);

// Pushes one argument per character of `fmt` and returns how many it pushed: 'b' boolean (an int,
// true when positive), 'd'/'f' number (a double, stored as float), 'i' integer (an int), 'o' object,
// 's' string, 't' table, 'v' vector (a pointer to three floats, copied into a new userdata).
// NON_MATCHING(minor): the six out-of-line va_arg register-area stubs at the end are laid out
// with case 'o' third instead of last (same instructions, branch targets permuted to match).
s32 fn_71010bf400(lua_State* L, const char* fmt, va_list args) {
    if (!fmt)
        return 0;
    u64 i = 0;
    for (; fmt[i] != 0; i++) {
        switch (fmt[i]) {
        case 'b':
            lua_pushboolean(L, va_arg(args, int) > 0);
            break;
        case 'd':
        case 'f':
            lua_pushnumber(L, (lua_Number)va_arg(args, double));
            break;
        case 'i':
            lua_pushinteger(L, va_arg(args, int));
            break;
        case 'o':
            fn_7100fbc4e0(L, va_arg(args, void*));
            break;
        case 's':
            lua_pushstring(L, va_arg(args, const char*));
            break;
        case 't':
            fn_71010bf700(L, va_arg(args, void*));
            break;
        case 'v': {
            const base::math::CVector3D* v = va_arg(args, const base::math::CVector3D*);
            base::math::CVector3D* out = fn_71010c0ff0(L);
            out->x = v->x;
            out->y = v->y;
            out->z = v->z;
            break;
        }
        default:
            return luaL_error(L, "Invalid parameter type in function call: '%c'", fmt[i]);
        }
    }
    return (s32)i;
}

// fn_71010bf400 with the arguments passed directly. On the port it must be compiled code, not a
// lift: a lifted variadic function hands on an AArch64 va_list, which the host's fn_71010bf400 cannot read.
s32 fn_71010c0b40(lua_State* L, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s32 n = fn_71010bf400(L, fmt, args);
    va_end(args);
    return n;
}

// Calls the script function `name` with the arguments `fmt` describes; whether the call succeeded.
bool fn_71010c0e70(const char* name, const char* fmt, ...) {
    if (!name || fn_71010c0bb0(g_unk_7172a01d18, name) == 0)
        return false;
    va_list args;
    va_start(args, fmt);
    s32 n = fn_71010bf400(g_unk_7172a01d18, fmt, args);
    lua_State* L = g_unk_7172a01d18;
    bool ok = true;
    if (fn_710109cef0(L, n, 1, 0) != 0) {
        lua_tostring(L, -1);
        ok = false;
    }
    lua_pop(L, 1);
    va_end(args);
    return ok;
}
