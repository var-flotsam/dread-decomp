// Unit 2618 (tools/tu_map.py, "lua_lib"): reads a typed pointer argument from the Lua stack (the reverse of
// fn_7100fbf55c in src/game/lua_bind/lua_lib_PushTyped_b49a6.cpp): a display object (fn_71010bef20) or a
// game object (fn_71010beee0) is read as itself and stored through the value (fn_710009d7a8, typed by
// fn_7100fc036c or fn_7100231b40); anything else, or a failed read, goes through the generic reader
// fn_7100fc22a0 with mode 2. Names and layout are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2620.h"
#include "game/lua_lib.h"
#include "types.h"

struct lua_State;

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkTypedRef;

// As src/clones/clones_06.cpp declares it.
struct UnkEnumType;
UnkEnumType* fn_7100231b40();  // as src/game_s7/fn_710024cc40.cpp declares it

bool fn_71010bef20(lua_State* L, void** out, s32 index);
bool fn_71010beee0(lua_State* L, void** out, s32 index);

struct UnkLuaReadScratch_b49a6 {
    u64 words[6];
};

struct UnkLuaArgReader2_b49a6 {
    u8 pad_0x0[0x10];
    lua_State* L;
    s32 index;
};

struct UnkPointerType2_b49a6 {
    u8 pad_0x0[0xe8];
    const CType* pointee;
};

static inline UnkPointerType2_b49a6* AsPointerType(const CType* type) {
    const UnkReflObject* o = reinterpret_cast<const UnkReflObject*>(type);
    return fn_710009d50c(o->vf_0x0(), fn_710009bedc()) ? reinterpret_cast<UnkPointerType2_b49a6*>(const_cast<CType*>(type))
                                                         : nullptr;
}

// Returns a word (the generic reader's bool, widened before the paths join).
u32 fn_7100fbfe1c(UnkLuaArgReader2_b49a6* self, UnkTypedRef* value) {
    u32 ok;
    lua_State* L = self->L;
    s32 index = self->index;
    const CType* pointee = AsPointerType(value->type)->pointee;
    if (fn_710009d50c(pointee, fn_71011c3868())) {
        void* object = nullptr;
        if (fn_71010bef20(L, &object, -1)) {
            const CType* type = value->type;
            UnkTypedRef src = {fn_7100fc036c(), &object};
            fn_710009d7a8(type, value, &src);
            ok = true;
            goto done;
        }
    } else if (fn_710009d50c(pointee, fn_7101043688())) {
        void* object = nullptr;
        if (fn_71010beee0(L, &object, -1)) {
            const CType* type = value->type;
            UnkTypedRef src = {reinterpret_cast<const CType*>(fn_7100231b40()), &object};
            fn_710009d7a8(type, value, &src);
            ok = true;
            goto done;
        }
    }
    {
    UnkLuaReadScratch_b49a6 scratch = {};
    ok = fn_7100fc22a0(L, index, value, 2, &scratch);
    }
done:
    return ok;
}
