// Script-call helpers next to UnkLuaCall.cpp: call a pushed function and read its result.
#include <string.h>

#include "../../lib/lua/lua.h"
#include "../../lib/lua/lauxlib.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2677.h"
#include "types.h"

s32 fn_71010befe0(lua_State* L, void* out, s32 idx);

// Calls the function on the stack with `nargs` arguments. On an error, copies the message into
// `err` (at most `errSize - 1` characters) and returns 0; otherwise reads the result into `out`.
s32 fn_71010c0cd0(lua_State* L, s32 nargs, char* err, u32 errSize, void* out) {
    s32 r;
    if (fn_710109cef0(L, nargs, 1, 0) != 0) {
        const char* msg = lua_tostring(L, -1);
        u32 len = strlen(msg);
        u32 n = errSize - 1 > len ? len : errSize - 1;
        memcpy(err, msg, n);
        err[n] = 0;
        r = 0;
    } else {
        r = fn_71010befe0(L, out, -1);
    }
    lua_pop(L, 1);
    return r;
}

// Calls the function on the stack with `nargs` arguments and reads its result as a number and as a
// present-or-nil flag. On an error, copies the message into `err` like fn_71010c0cd0, clears both
// outputs and returns false.
bool fn_71010c0d80(lua_State* L, s32 nargs, char* err, u32 errSize, s32* outNumber, bool* outPresent) {
    bool ok;
    if (fn_710109cef0(L, nargs, 1, 0) != 0) {
        const char* msg = lua_tostring(L, -1);
        u32 len = strlen(msg);
        u32 n = errSize - 1 > len ? len : errSize - 1;
        memcpy(err, msg, n);
        err[n] = 0;
        if (outPresent)
            *outPresent = false;
        ok = false;
        if (outNumber)
            *outNumber = 0;
    } else {
        if (outPresent)
            *outPresent = lua_type(L, -1) != LUA_TNIL;
        if (outNumber)
            *outNumber = (s32)lua_tonumber(L, -1);
        ok = true;
    }
    lua_pop(L, 1);
    return ok;
}
