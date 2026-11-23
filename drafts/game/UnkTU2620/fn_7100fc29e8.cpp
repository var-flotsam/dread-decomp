// Unit 2620 (tools/tu_map.py): steps the script reader to the named entry and reads it into a typed value:
// a plain value, the type name a table carries in its "__Type" field, or the number of entries of a table
// (string keys equal to one reserved id not counted). Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkTypedRef;

// A typed pointer to a value the reader fills.
struct UnkReadArg_b0249 {
    const base::reflection::CType* type;
    u64* value;
};

// The reader as this step sees it (unit 2619's table walker).
struct UnkLuaReader_fc29e8 {
    lua_State* L;  // 0x0
    s32 base_index;  // 0x8
    u8 pad_0xc[0x4];
    s32 depth;  // 0x10
    u8 pad_0x14[0xc];
    s32 mode;  // 0x20
};
static_assert(__builtin_offsetof(UnkLuaReader_fc29e8, mode) == 0x20, "UnkLuaReader_fc29e8 layout");

// The script object a userdata holds: its type id at +8.
struct UnkLuaObject_fc29e8 {
    u8 pad_0x0[0x8];
    CStrId type;
};

struct UnkLuaWalker_b0253;
bool fn_7100fc18bc(UnkLuaWalker_b0253* self, s32 level, const CStrId& key);
bool fn_7100fbded0(lua_State* L, UnkReadArg_b0249* arg, s32 index);
UnkLuaObject_fc29e8* fn_7100fbdf20(lua_State* L, s32 index);

// The reserved key not counted among a table's entries.
extern CStrId g_unk_71729eff28;

// Stores `src` (of type `type`) into the value the argument points to.
template <const base::reflection::CType* (*GetType)()>
static inline void Store(UnkReadArg_b0249* arg, void* src) {
    const base::reflection::CType* type = arg->type;
    UnkTypedRef ref = {GetType(), src};
    fn_710009d7a8(type, reinterpret_cast<UnkTypedRef*>(arg), &ref);
}

// The string id of the key at `index` (the empty id when it is not a string).
static inline CStrId KeyAt(lua_State* L, s32 index) {
    CStrId key(base::global::UnkStrIdInline(), nullptr, false);
    if (lua_isstring(L, index))
        key = CStrId(luaL_checklstring(L, index, nullptr), true);
    return key;
}

bool fn_7100fc29e8(void* reader, u32 flags, UnkReadArg_b0249* arg, const CStrId* name) {
    UnkLuaReader_fc29e8* self = static_cast<UnkLuaReader_fc29e8*>(reader);
    if (!fn_7100fc18bc(static_cast<UnkLuaWalker_b0253*>(reader), flags, *name))
        return false;

    switch (self->mode) {
    case 0:
        if (self->depth <= 0) {
            Store<fn_7100009658>(arg, &g_unk_7101d470e0);
            return true;
        }
        fn_7100fbded0(self->L, arg, -2);
        return false;

    case 1: {
        if (self->depth >= 1) {
            s32 index = lua_gettop(self->L);
        lua_type(self->L, index);
        lua_getfield(self->L, index, "__Type");
        bool found = false;
        if (lua_type(self->L, -1) == LUA_TSTRING) {
            fn_7100fbded0(self->L, arg, -1);
            found = true;
        }
        lua_settop(self->L, -2);
        return found;
        }
        UnkLuaObject_fc29e8* object = fn_7100fbdf20(self->L, -1);
        Store<fn_7100009658>(arg, &object->type);
        return false;
    }

    case 2: {
        s32 index = self->depth >= 1 ? lua_gettop(self->L) : self->base_index;
        if (lua_type(self->L, index) != LUA_TTABLE)
            return fn_7100fbded0(self->L, arg, index);
        u32 count = 0;
        lua_pushnil(self->L);
        while (lua_next(self->L, index)) {
            if (lua_type(self->L, -2) != LUA_TSTRING || !(KeyAt(self->L, -2) == g_unk_71729eff28))
                ++count;
            lua_settop(self->L, -2);
        }
        Store<fn_7100005178>(arg, &count);
        return true;
    }
    }
    return false;
}
