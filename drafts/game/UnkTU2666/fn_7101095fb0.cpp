// Unit 2666 Lua binding: pushes a property bag holding four zeroed values ("LR", "LF", "RR", "RF")
// as a table. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../../lib/lua/lauxlib.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "../../../src/game_s7/UnkPropBagS7.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringPool;
using base::global::UnkStringData;
using base::reflection::UnkReflValue;
using unk_prop_bag::UnkPropBag;

int fn_71010c0b40(lua_State* L, const char* fmt, ...);

s32 fn_710108ea80(lua_State* L) {
    UnkPropBag bag;
    fn_710033187c(&bag, CStrId("LR", true), 0.0f);
    fn_710033187c(&bag, CStrId("LF", true), 0.0f);
    fn_710033187c(&bag, CStrId("RR", true), 0.0f);
    fn_710033187c(&bag, CStrId("RF", true), 0.0f);
    return fn_71010c0b40(L, "t", &bag);
}

// ---- a reflected path read from a script string ----

struct UnkPathType_b0165 {
    u8 pad_0x0[0x40];
    void (*destroy)(void* arg);  // 0x40
};

// One argument of a path: a value and its type, destroyed through the type.
struct UnkPathArg_b0165 {
    u64 value;
    UnkPathType_b0165* type;
    ~UnkPathArg_b0165() {
        if (type) {
            type->destroy(this);
            type = nullptr;
        }
    }
};

// A path of arguments, built by fn_7100ad3118 from the text of a name (so it starts out unconstructed).
union UnkPath_b0165 {
    base::UnkVector<UnkPathArg_b0165> args;
    UnkPath_b0165() {}
    ~UnkPath_b0165() {}
};

// The name's text sits at +0x10 of the pooled string; a use count at +0x44.
struct UnkNameData_b0165 {
    u8 pad_0x0[0x10];
    char text[0x34];
    u32 use_count;  // 0x44
};

// A resolved value, as fn_7100ad7cc0 returns it in memory.
struct UnkResolved_b0165 {
    UnkReflValue value;
    u8 unk_0x18[0x18];
    ~UnkResolved_b0165() { fn_71000a1c60(&value); }
};

struct UnkValueArg_b0165 {
    u8 storage[0x18];
};

struct UnkTypedPair_b0157;

void fn_7100ad3118(UnkPath_b0165* out, const char* text);
UnkResolved_b0165 fn_7100ad7cc0(void* path);
void fn_71000a1710(UnkValueArg_b0165* out, const UnkReflValue* value);
s32 fn_7100fbde70(void* state, UnkTypedPair_b0157* value, s32 arg0, s32 arg1);

// The name's id, released at the end of the function.
struct UnkNameId_b0165 : UnkStrIdBase {
    UnkNameId_b0165() : UnkStrIdBase(base::global::UnkStrIdInline(), nullptr, false) {}
    ~UnkNameId_b0165() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if ((&pool->empty != data) & (data != nullptr)) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkResolvedSlot_b0165 {
    union {
        UnkResolved_b0165 value;
    };
    UnkResolvedSlot_b0165() {}
    ~UnkResolvedSlot_b0165() { value.~UnkResolved_b0165(); }
};

s32 fn_7101095fb0(lua_State* L) {
    UnkNameId_b0165 name;
    if (!lua_isstring(L, -1))
        return 0;
    UnkResolvedSlot_b0165 resolved;
    {
        UnkStrIdBase id(luaL_checklstring(L, -1, nullptr), true);
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (name.data && name.data != &pool->empty) {
                --name.data->ref_count;
                if (name.data->ref_count == 0) {
                    fn_71000803a4(pool, name.data);
                    pool = g_unk_7101d47108;
                }
            }
            name.data = &pool->empty;
        }
        name.data = id.data;
    }
    UnkNameData_b0165* text = reinterpret_cast<UnkNameData_b0165*>(name.data);
    ++text->use_count;
    {
        UnkPath_b0165 path;
        fn_7100ad3118(&path, text->text);
        new (&resolved.value) UnkResolved_b0165(fn_7100ad7cc0(&path));
        path.args.Free();
    }
    s32 result;
    if (resolved.value.value.type) {
        UnkValueArg_b0165 arg;
        fn_71000a1710(&arg, &resolved.value.value);
        result = fn_7100fbde70(L, reinterpret_cast<UnkTypedPair_b0157*>(&arg), resolved.value.value.field_0x14, 0);
    } else {
        result = 0;
    }
    return result;
}
