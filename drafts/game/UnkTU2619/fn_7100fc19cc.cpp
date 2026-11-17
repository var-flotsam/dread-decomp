// Unit 2619 (a Lua table walker): moves the walk to the entry of the current table whose string key is
// the given id, makes it the current key and records it as passed. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "types.h"

// Needs the walker's keys at 0x28 to be a base::UnkStrIdVec (count, capacity, step) and a bool return;
// see the worker report.

using base::global::CStrId;
using base::global::UnkStrIdInline;

struct UnkLuaWalker_b0253 {
    lua_State* L;  // 0x0
    s32 base_index;  // 0x8
    u8 pad_0xc[0x4];
    s32 depth;  // 0x10
    u8 pad_0x14[0x4];
    CStrId key;  // 0x18
    s32 level;  // 0x20
    u8 pending;  // 0x24
    u8 pad_0x25[0x3];
    base::UnkStrIdVec keys;  // 0x28

    // The stack index the cursor is at: the top while it is inside a table, else the base.
    s32 Top() {
        if (depth >= 1)
            return lua_gettop(L);
        return base_index;
    }
    // One below that, for the entry the walk steps back to.
    s32 Below() {
        if (depth >= 1)
            return lua_gettop(L) - 1;
        return base_index;
    }
};
static_assert(__builtin_offsetof(UnkLuaWalker_b0253, keys) == 0x28, "UnkLuaWalker_b0253 layout");

bool fn_7100fc19cc(UnkLuaWalker_b0253* self, const CStrId& key) {
    if (self->key == key)
        return true;
    if (self->keys.count != 0) {
        s32 below = lua_gettop(self->L) - 2;
        lua_type(self->L, below);
        lua_settop(self->L, -3);
    }
    s32 index = self->Top();
    if (lua_type(self->L, index) != LUA_TTABLE)
        return false;
    lua_pushnil(self->L);
    bool found = false;
    while (lua_next(self->L, index)) {
        if (lua_type(self->L, -2) == LUA_TSTRING) {
            CStrId id(UnkStrIdInline(), nullptr, false);
            lua_State* L = self->L;
            if (lua_isstring(L, -2))
                id = CStrId(luaL_checklstring(L, -2, nullptr), true);
            if (id == key) {
                self->key = id;
                self->pending = 1;
                self->keys.PushBack(key);
                found = true;
            }
        }
        if (found)
            return true;
        lua_settop(self->L, -2);
    }
    return false;
}
