// Unit 2618 (tools/tu_map.py, "lua_lib"): reads the Lua arguments from `first` on as reflected values,
// one for each type in the list, through the unit's argument reader (fn_71000a1784 walks each value with
// it); it stops at the first missing argument. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "base/global/CRntVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CRntVector;
using base::reflection::CType;
using base::reflection::UnkReflValue;

// A reflected value owned by the array: empty until a type is set (fn_71000a2014), moved with
// fn_71000a179c and destroyed with fn_71000a1c60.
struct UnkLuaArgValue_lfbe580 {
    UnkReflValue value;

    UnkLuaArgValue_lfbe580() {
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
    }
    UnkLuaArgValue_lfbe580(UnkLuaArgValue_lfbe580&& other) { fn_71000a179c(&value, &other.value); }
    ~UnkLuaArgValue_lfbe580() { fn_71000a1c60(&value); }
};
static_assert(sizeof(UnkLuaArgValue_lfbe580) == 0x18, "UnkLuaArgValue_lfbe580 layout");

// The types to read, in order.
struct UnkTypeList_lfbe580 {
    const CType* const* data;
    u64 count;
};

// A value's type and where the value lives.
struct UnkTypedPair_lfbe580 {
    const char* type;
    void* obj;
};

// The unit's argument reader (vtable 0x7101cab7c0): reads the Lua argument at `index` into each value
// it visits.
extern void* g_unk_7101cab7c0[] __attribute__((visibility("hidden")));

struct UnkLuaArgReader_lfbe580 {
    void** vtable;
    u32 kind;
    lua_State* L;
    int index;
};

// Where the value lives: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValuePtr(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || reinterpret_cast<const CType*>(v->type)->m_size > 8)
        return v->field_0x0;
    return v;
}

CRntVector<UnkLuaArgValue_lfbe580> fn_7100fbe580(lua_State* L, const UnkTypeList_lfbe580* types, int first) {
    CRntVector<UnkLuaArgValue_lfbe580> args(0xf);
    for (const CType* const *it = types->data, *const *end = it + types->count; it != end; ++it) {
        int index = first + args.count;
        if (lua_type(L, index) == LUA_TNONE)
            break;
        UnkLuaArgReader_lfbe580 reader;
        UnkLuaArgValue_lfbe580 arg;
        fn_71000a2014(&arg.value, *it);
        UnkTypedPair_lfbe580 pair = {arg.value.type, ValuePtr(&arg.value)};
        reader.kind = 0;
        reader.vtable = g_unk_7101cab7c0;
        reader.L = L;
        reader.index = index;
        fn_71000a1784(reinterpret_cast<UnkFormatArgs*>(&pair), reinterpret_cast<UnkTextBuffer200*>(&reader));
        args.EmplaceBack(static_cast<UnkLuaArgValue_lfbe580&&>(arg));
    }
    return args;
}
