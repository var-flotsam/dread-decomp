// Unit 2619 (a Lua table walker, no reliable name): a cursor over the tables on a script stack that
// keeps the string ids of the keys it has passed. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

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

bool fn_7100fc1cd8(UnkLuaWalker_b0253* self, s32 index);
void fn_7100fc19cc(UnkLuaWalker_b0253* self, const CStrId& key);

void fn_7100fc20d4(UnkLuaWalker_b0253* self) {
    for (u64 i = 0; i < self->key_count; ++i)
        self->keys[i].Reset();
    self->key_count = 0;
    self->key = g_unk_7101d470e0;
}

bool fn_7100fc1f4c(UnkLuaWalker_b0253* self) {
    for (;;) {
        if (lua_type(self->L, self->Top()) == LUA_TNONE)
            return false;
        s32 index = self->Top();
        if (lua_type(self->L, index) == LUA_TTABLE) {
            lua_pushnil(self->L);
            if (fn_7100fc1cd8(self, index)) {
                ++self->depth;
                fn_7100fc20d4(self);
                return true;
            }
            fn_7100fc20d4(self);
        }
        while (lua_type(self->L, self->Top()) != LUA_TNONE) {
            lua_settop(self->L, -2);
            if (fn_7100fc1cd8(self, self->Below()))
                return true;
            fn_7100fc20d4(self);
            --self->depth;
        }
    }
}

bool fn_7100fc18bc(UnkLuaWalker_b0253* self, s32 level, const CStrId& key) {
    if (key.data == &g_unk_7101d47108->empty) {
        s32 previous = self->level;
        self->level = level;
        if (previous >= level) {
            if (self->pending) {
                self->pending = 0;
                s32 below = lua_gettop(self->L) - 2;
                lua_type(self->L, below);
                lua_settop(self->L, -3);
                s32 index = self->Top();
                lua_pushnil(self->L);
                if (fn_7100fc1cd8(self, index))
                    return lua_type(self->L, self->Top()) != LUA_TNONE;
            }
            fn_7100fc1f4c(self);
        }
    } else {
        fn_7100fc19cc(self, key);
        self->level = level;
    }
    return lua_type(self->L, self->Top()) != LUA_TNONE;
}
