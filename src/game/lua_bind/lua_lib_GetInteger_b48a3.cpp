// Unit 2618 (tools/tu_map.py, "lua_lib"): two argument readers for 64-bit integer types, near copies of
// the number reader fn_7100fbf914: when the argument at the reader's index is a number, it is read as
// an int (luaL_checkint) and stored widened. Names and layout are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "types.h"

struct UnkLuaArgReader_b48a3 {
    u8 pad_0x0[0x10];
    lua_State* L;
    int index;
};

static inline bool UnkReadInt_b48a3(UnkLuaArgReader_b48a3* reader, s64* out) {
    lua_State* L = reader->L;
    int index = reader->index;
    if (!lua_isnumber(L, index))
        return false;
    *out = luaL_checkint(L, index);
    return true;
}

bool fn_7100fbfa58(UnkLuaArgReader_b48a3* reader, s64* out) {
    return UnkReadInt_b48a3(reader, out);
}

bool fn_7100fbfb9c(UnkLuaArgReader_b48a3* reader, u64* out) {
    return UnkReadInt_b48a3(reader, reinterpret_cast<s64*>(out));
}
