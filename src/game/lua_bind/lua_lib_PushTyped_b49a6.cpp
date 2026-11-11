// Unit 2618 (tools/tu_map.py, "lua_lib"): pushes a typed value (a type descriptor and a pointer) onto the
// Lua stack as a full userdata holding a reflected value (kind 6, stored through its pointer) with the
// "__TypedValueType" metatable. In mode 1, and in mode 0 for a value that isn't a plain type, enum or flag
// set (fn_710009c7b8, fn_710009921c, fn_710009a11c) when it is a pointer or `owned` is set, the value is
// then handed on with fn_710109cfb0. Names and layout are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/lua_lib.h"
#include "types.h"

using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflValue;

struct UnkTypedValue_b49a6 {
    UnkReflObject* type;
    void* ptr;
};

void fn_710109cfb0(UnkReflValue* value);

static inline bool HandsOn(UnkReflObject* type, u32 owned) {
    const CType* cls = type->vf_0x0();
    if (cls == fn_710009c7b8() || cls == fn_710009921c() || cls == fn_710009a11c())
        return false;
    bool pointer = fn_710009d50c(type->vf_0x0(), fn_710009bedc());
    return owned || pointer;
}

bool fn_7100fbd320(lua_State* L, UnkTypedValue_b49a6* value, u32 owned, s32 mode) {
    UnkReflValue* v = static_cast<UnkReflValue*>(lua_newuserdata(L, sizeof(UnkReflValue)));
    v->field_0x0 = nullptr;
    v->type = nullptr;
    v->kind = 6;
    v->field_0x14 = 1;
    luaL_getmetatable(L, "__TypedValueType");
    lua_setmetatable(L, -2);
    UnkReflObject* type = value->type;
    void* ptr = value->ptr;
    fn_71000a1c60(v);
    fn_71000a18d4(v, reinterpret_cast<const CType*>(type), ptr, owned);
    if (mode == 1 || (mode == 0 && HandsOn(value->type, owned)))
        fn_710109cfb0(v);
    return true;
}

// The pointer type's view (CPointerType, fn_710009bedc): whether the pointee is owned (+0xe0) and the
// pointee's type (+0xe8).
struct UnkPointerType_b49a6 {
    u8 pad_0x0[0xe0];
    u8 owned;
    u8 pad_0xe1[7];
    UnkReflObject* pointee;
};

struct UnkLuaPusher_b49a6 {
    u8 pad_0x0[0x10];
    lua_State* L;
    u8 pad_0x18[4];
    s32 mode;    // 0x1c
    u32 pushed;  // 0x20
};

bool fn_71010befa0(lua_State* L, void* object);
bool fn_71010befc0(lua_State* L, void* object);

static inline UnkPointerType_b49a6* AsPointerType(UnkReflObject* type) {
    return fn_710009d50c(type->vf_0x0(), fn_710009bedc()) ? reinterpret_cast<UnkPointerType_b49a6*>(type)
                                                             : nullptr;
}

// Pushes a typed pointer: a display object (fn_71010befa0) or game object (fn_71010befc0) as itself,
// anything else as a typed value (fn_7100fbd320); counts what it pushed.
bool fn_7100fbf55c(UnkLuaPusher_b49a6* self, UnkTypedValue_b49a6* value) {
    lua_State* L = self->L;
    s32 mode = self->mode;
    UnkReflObject* pointee = AsPointerType(value->type)->pointee;
    bool ok;
    if (fn_710009d50c(reinterpret_cast<const CType*>(pointee), fn_71011c3868())) {
        ok = fn_71010befa0(L, *static_cast<void**>(value->ptr));
    } else if (fn_710009d50c(reinterpret_cast<const CType*>(pointee), fn_7101043688())) {
        ok = fn_71010befc0(L, *static_cast<void**>(value->ptr));
    } else {
        fn_7100fbd320(L, value, AsPointerType(value->type)->owned, mode);
        ok = true;
    }
    if (!ok)
        return false;
    ++self->pushed;
    return true;
}

// Pushes an enum value as its name (fn_71000999cc on the enum type, CEnumType fn_710009921c); counts it.
using base::global::CStrId;

CStrId fn_71000999cc(UnkReflObject* enum_type, u32 value);

struct UnkCountedStrData_b49a6 {
    u8 pad_0x0[0x10];
    const char* str;
    u8 pad_0x18[0x2c];
    union {
        s32 field_0x44;
    };
};

// Reading the string bumps the data's use count (as in src/base/reflection/CType/fn_71000a0660.cpp).
static inline const char* CStr(const CStrId& id) {
    auto* data = reinterpret_cast<UnkCountedStrData_b49a6*>(id.data);
    ++data->field_0x44;
    return data->str ? data->str : "";
}

bool fn_7100fbf690(UnkLuaPusher_b49a6* self, UnkTypedValue_b49a6* value) {
    lua_State* L = self->L;
    UnkReflObject* type = value->type;
    UnkReflObject* enum_type = type && fn_710009d50c(type->vf_0x0(), fn_710009921c()) ? type : nullptr;
    {
        CStrId name = fn_71000999cc(enum_type, *static_cast<u32*>(value->ptr));
        lua_pushstring(L, CStr(name));
    }
    ++self->pushed;
    return true;
}
