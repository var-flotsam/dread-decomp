// Unit 2665 (tools/tu_map.py, "lua_lib"): the display objects the Lua scripts see, full userdata with
// the "__DisplayObjectType" metatable holding a counted weak handle on a GUI display object.
// fn_71010757f0 runs a protected call and takes the display object it returns (or copies the error
// message), fn_7101075950 pushes the object a path names in the GUI root, fn_7101075ad0 pushes an
// object, fn_71010762d0 is __eq and fn_7101075c30 calls the reflected method named by the closure's
// upvalue on the object. Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU2668.h"
#include "game/UnkTU2676.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::UnkReflValue;

UnkGuiDisplayObject* fn_71011b5d3c(UnkGuiRoot* root, const char* const* path);
void fn_710007c144(UnkHashStream_0x38* s);

// A display object as the scripts hold it: full userdata with the "__DisplayObjectType" metatable
// holding a counted weak handle on the object.
struct UnkLuaDisplayObject_ovb0109 {
    UnkWeakHandle* handle;

    UnkWeakTarget* Get() const { return handle ? handle->target : nullptr; }
};

static inline UnkLuaDisplayObject_ovb0109* ToDisplayObject(lua_State* L, int index) {
    return static_cast<UnkLuaDisplayObject_ovb0109*>(fn_71010a4740(L, index, "__DisplayObjectType"));
}

__attribute__((always_inline)) static inline void PushDisplayObject(lua_State* L, UnkWeakTarget* object) {
    auto* ud = static_cast<UnkLuaDisplayObject_ovb0109*>(lua_newuserdata(L, 8));
    ud->handle = nullptr;
    lua_getfield(L, LUA_REGISTRYINDEX, "__DisplayObjectType");
    lua_setmetatable(L, -2);
    AssignRef(&ud->handle, object ? GetWeakHandle(object) : nullptr);
}

s32 fn_71010757f0(lua_State* L, s32 nargs, char* error, u32 error_size, UnkWeakTarget** out) {
    s32 result;
    if (fn_710109cef0(L, nargs, 1, 0) != 0) {
        const char* message = lua_tolstring(L, -1, nullptr);
        u32 length = __builtin_strlen(message);
        u32 n = error_size - 1 > length ? length : error_size - 1;
        __builtin_memcpy(error, message, n);
        error[n] = '\0';
        result = 0;
    } else {
        if (lua_type(L, -1) == LUA_TUSERDATA) {
            if (UnkLuaDisplayObject_ovb0109* ud = ToDisplayObject(L, -1)) {
                *out = ud->Get();
                result = 1;
                goto done;
            }
            luaL_typerror(L, -1, "__DisplayObjectType");
        }
        result = 0;
        error[0] = '\0';
    }
done:
    lua_settop(L, -2);
    return result;
}

int fn_7101075950(lua_State* L, const char* path) {
    UnkGuiRoot* root = g_unk_717271e8f8;
    UnkGuiDisplayObject* object;
    {
        CRntString name(path);
        object = fn_71011b5d3c(root, reinterpret_cast<const char* const*>(&name));
    }
    if (!object)
        return 0;
    PushDisplayObject(L, reinterpret_cast<UnkWeakTarget*>(object));
    return 1;
}

int fn_7101075ad0(lua_State* L, UnkWeakTarget* object) {
    PushDisplayObject(L, object);
    return 1;
}

// __eq: whether two display objects are the same object.
int fn_71010762d0(lua_State* L) {
    UnkLuaDisplayObject_ovb0109* a = nullptr;
    if (lua_type(L, 1) == LUA_TUSERDATA) {
        a = ToDisplayObject(L, 1);
        if (!a)
            luaL_typerror(L, 1, "__DisplayObjectType");
    }
    UnkLuaDisplayObject_ovb0109* b = nullptr;
    if (lua_type(L, 2) == LUA_TUSERDATA) {
        b = ToDisplayObject(L, 2);
        if (!b)
            luaL_typerror(L, 2, "__DisplayObjectType");
    }
    if (!a->Get() || !b->Get()) {
        luaL_error(L, "DisplayObject has been deleted!");
        return 0;
    }
    lua_pushboolean(L, a->Get() == b->Get());
    return 1;
}

// A reflected object as seen through its class getter (vtable slot 0).
struct UnkReflObject_ovb0109 {
    virtual char* GetClass();
};

// The call's arguments read from the Lua stack: a file the values were read through and the values.
struct UnkLuaArgs_ovb0109 : UnkHashStream_0x38 {
    union {
        base::UnkVector<UnkReflValue> values;  // 0x38
    };
    ~UnkLuaArgs_ovb0109() {
        fn_710007c144(this);
        if (values.data) {
            for (u32 i = 0; i < values.count; ++i)
                fn_71000a1c60(&values.data[i]);
            values.count = 0;
            values.allocator->vf_0x48(values.data);
            values.capacity = 0;
        }
        fn_710007c0e4(this);
    }
};

UnkLuaArgs_ovb0109* fn_710109ead0(lua_State* L, int first);
void* fn_71000978e4(char* cls, const CStrId* name);
struct UnkGameObjectTyped_ovb0115;  // a typed pointer (src/game/lua_lib/lua_lib_GameObjectCall_ovb0115.cpp)
int fn_7100fbe7b0(lua_State* L, UnkGameObjectTyped_ovb0115* value, CStrId* name);

static inline char* AsClass(char* type) {
    return type && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(
                                     reinterpret_cast<UnkReflObject_ovb0109*>(type)->GetClass()),
                                 reinterpret_cast<const base::reflection::CType*>(fn_7100096100()))
               ? type
               : nullptr;
}

int fn_7101075c30(lua_State* L) {
    const char* method = luaL_checkstring(L, lua_upvalueindex(1));
    luaL_checktype(L, 1, LUA_TUSERDATA);
    UnkLuaDisplayObject_ovb0109* ud = ToDisplayObject(L, 1);
    if (!ud) {
        luaL_typerror(L, 1, "__DisplayObjectType");
        return 0;
    }
    UnkReflObject_ovb0109* object = reinterpret_cast<UnkReflObject_ovb0109*>(ud->Get());
    if (!object)
        return 0;
    UnkLuaArgs_ovb0109* args = fn_710109ead0(L, 2);
    base::reflection::UnkReflTypedPtr typed;
    typed.type = fn_710009dcd8(object->GetClass(), object);
    typed.ptr = object;
    CStrId name(method, true);
    int result = 0;
    if (fn_71000978e4(AsClass(typed.type), &name))
        result = fn_7100fbe7b0(L, reinterpret_cast<UnkGameObjectTyped_ovb0115*>(&typed), &name);
    delete args;
    return result;
}
