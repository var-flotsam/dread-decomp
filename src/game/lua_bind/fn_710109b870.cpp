// Unit 2667 (lua_bind): the display object a script argument names: a "__DisplayObjectType"
// userdata's weak handle target, or a path looked up in the GUI root. Names and layouts are
// placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/global/CRntString.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2744.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;

// NON_MATCHING(functional): Control flow differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkGuiDisplayObject* fn_710109b870(lua_State* L, int index) {
    UnkGuiDisplayObject* object = nullptr;
    int type = lua_type(L, index);
    if (type == LUA_TSTRING) {
        const char* path = luaL_checkstring(L, index);
        UnkGuiRoot* root = g_unk_717271e8f8;
        CRntString name(path);
        object = fn_71011b5d3c(root, reinterpret_cast<const char* const*>(&name));
    } else if (type == LUA_TUSERDATA) {
        UnkWeakHandle** ud = static_cast<UnkWeakHandle**>(luaL_checkudata(L, index, "__DisplayObjectType"));
        if (ud != nullptr && *ud != nullptr)
            return reinterpret_cast<UnkGuiDisplayObject*>((*ud)->target);
    }
    return object;
}