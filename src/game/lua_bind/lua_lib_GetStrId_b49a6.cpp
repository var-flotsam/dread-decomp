// Unit 2618 (tools/tu_map.py, "lua_lib"): the string-id argument reader (beside the integer readers of
// src/game/lua_bind/lua_lib_GetInteger_b48a3.cpp): when the argument at the reader's index is a string,
// it is read (luaL_checklstring) and interned as an id (fn_71000003d4) that replaces `out`'s, the old id
// released. Names and layout are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStrIdBase;
using base::global::UnkStringPool;

struct UnkLuaArgReader_b49a6 {
    u8 pad_0x0[0x10];
    lua_State* L;
    int index;
};

bool fn_7100fbfd20(UnkLuaArgReader_b49a6* reader, UnkStrIdBase* out) {
    lua_State* L = reader->L;
    int index = reader->index;
    if (!lua_isstring(L, index))
        return false;
    UnkStrIdBase id(luaL_checkstring(L, index), true);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (out->data && out->data != &pool->empty) {
            --out->data->ref_count;
            if (out->data->ref_count == 0) {
                fn_71000803a4(pool, out->data);
                pool = g_unk_7101d47108;
            }
        }
        out->data = &pool->empty;
    }
    out->data = id.data;
    return true;
}
