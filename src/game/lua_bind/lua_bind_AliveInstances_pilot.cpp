// A Lua binding: reads a string argument, asks the game manager about the string id built from it
// (fn_7100deb830) and pushes the integer answer. Names are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct lua_State;

s32 fn_71010c0270(lua_State* L, const char* fmt, ...);
int fn_71010c0b40(lua_State* L, const char* fmt, ...);
s32 fn_7100deb830(UnkGameManager* manager, const CStrId& id);

s32 fn_710107ee80(lua_State* L) {
    const char* name;
    if (!fn_71010c0270(L, "s", &name))
        return 0;
    UnkGameManager* manager = g_unk_7101d47310;
    s32 result;
    {
        CStrId id(name, true);
        result = fn_7100deb830(manager, id);
    }
    return fn_71010c0b40(L, "i", result);
}
