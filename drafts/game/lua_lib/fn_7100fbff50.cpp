// Unit 2618 (tools/tu_map.py, "lua_lib"): the typed-value reader's enum case: the Lua argument at the
// reader's index is read as a string id (the reader of fn_7100fbfd20, inlined) and the enumerator of that
// name is stored in the value (fn_710009996c). Names and layouts are placeholders.
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;
using base::global::UnkStringPool;
using base::reflection::CEnumType;
using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkLuaArgReader_lfbff50 {
    u8 pad_0x0[0x10];
    lua_State* L;
    int index;
};

// A reflected value: its type and its storage.
struct UnkEnumValueRef {
    void* field_0x0;
    s32* value;
};

void fn_710009996c(const CEnumType* self, const UnkEnumValueRef* out, const CStrId* name);

// The string argument at `index` as an id, replacing `out`'s (as fn_7100fbfd20 reads it).
static inline bool ReadStrId(lua_State* L, int index, UnkStrIdBase* out) {
    if (!lua_isstring(L, index))
        return false;
    UnkStrIdBase id(luaL_checkstring(L, index), true);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (out->data && out->data != &pool->empty) {
            --out->data->ref_count;
            if (out->data->ref_count == 0) {
                StringPool_Release(pool, out->data);
                pool = g_unk_7101d47108;
            }
        }
        out->data = &pool->empty;
    }
    out->data = id.data;
    return true;
}

// The value's type as an enum type, or null when it is not one.
static inline const CEnumType* AsEnumType(void* type) {
    if (!type)
        return nullptr;
    const CType* cls = reinterpret_cast<const CType*>(static_cast<UnkReflObject*>(type)->vf_0x0());
    if (fn_710009d50c(cls, fn_710009921c()))
        return static_cast<const CEnumType*>(type);
    return nullptr;
}

bool fn_7100fbff50(UnkLuaArgReader_lfbff50* reader, UnkEnumValueRef* value) {
    lua_State* L = reader->L;
    int index = reader->index;
    // Needs CStrId's default constructor (the empty id), which base/global/CStrId.h does not declare yet.
    CStrId name;
    if (ReadStrId(L, index, &name)) {
        fn_710009996c(AsEnumType(value->field_0x0), value, &name);
        return true;
    }
    return false;
}
