// Unit 2619 (a Lua table walker): steps the walk to the next string key of a table that is neither the
// skipped id nor one of the keys already passed, and makes it the current key. Names and layouts are
// placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/global/CStrId.h"
#include "types.h"

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
    CStrId* keys;  // 0x28
    u32 key_count;  // 0x30

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
static_assert(__builtin_offsetof(UnkLuaWalker_b0253, key_count) == 0x30, "UnkLuaWalker_b0253 layout");

// The id of keys the walk skips.
extern CStrId g_unk_71729eff28;

static inline CStrId* FindKey(UnkLuaWalker_b0253* self, const CStrId& id) {
    CStrId* it = self->keys;
    for (u32 n = self->key_count; n != 0; --n, ++it) {
        if (*it == id)
            return it;
    }
    return &self->keys[self->key_count];
}

bool fn_7100fc1cd8(UnkLuaWalker_b0253* self, s32 index) {
    if (lua_type(self->L, index) != LUA_TTABLE)
        return false;
    while (lua_next(self->L, index)) {
        if (lua_type(self->L, -2) != LUA_TSTRING)
            return true;
        {
            CStrId id(UnkStrIdInline(), nullptr, false);
            lua_State* L = self->L;
            if (lua_isstring(L, -2))
                id = CStrId(luaL_checklstring(L, -2, nullptr), true);
            if (!(id == g_unk_71729eff28) && FindKey(self, id) == &self->keys[self->key_count]) {
                self->key = id;
                return true;
            }
        }
        lua_settop(self->L, -2);
    }
    return false;
}
