// CGameObject's unit (tools/tu_map.py, unit 2650): sets a reflected field of an object from a Lua argument;
// the field name's first letter says the type (b bool, e enum by name, f float, i int, s string, v vector).
// Names and layouts are placeholders.
#include <string.h>
#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CEnumType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkTU2676.h"
#include "game/UnkTU85.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CRntString;
using base::global::UnkStringPool;
using base::global::UnkStrIdBase;
using base::reflection::CEnumType;
using base::reflection::CType;

bool CEnumType_TryGetValueByNameId(const CEnumType* self, const base::global::CStrId* name, s32* out);

struct UnkField_l043a04 {
    u8 pad_0x0[0x10];
    const base::reflection::UnkReflObject* type;  // 0x10, a reflected type descriptor
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkField_l043a04) == 0xb0, "layout");

// A class's fields, by the hash of their names.
struct UnkFieldTable_l043a04 {
    u8 pad_0x0[0xe8];
    UnkField_l043a04* fields;  // 0xe8
    u64* keys;                 // 0xf0
    u32 count;                 // 0xf8

    UnkField_l043a04* End() { return fields + count; }
    UnkField_l043a04* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &fields[i];
        return End();
    }
    // The field with that name hash, null when there is none.
    UnkField_l043a04* FindField(u64 key) {
        UnkField_l043a04* it = Find(key);
        return it != End() ? it : nullptr;
    }
};

static inline UnkFieldTable_l043a04* Fields_l043a04(UnkCameraCtrl* object) {
    return reinterpret_cast<UnkFieldTable_l043a04*>(const_cast<base::reflection::CClass*>(object->vf_0x0()));
}

// A typed pointer: the object with its class, or a value with its type.
struct UnkTyped_l043a04 {
    const void* type;
    void* ptr;
};

// The enum value's name as an id: released as CStrId's, but the count is decremented and tested in one step.
struct UnkEnumName_l043a04 : UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;
    ~UnkEnumName_l043a04() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

static inline u64 Hash_l043a04(const char* name) {
    return StringCrc64(name, strlen(name));
}

// Sets the field named by `key` from the value `in` points at, of the type `Type` gives.
template <const CType* (*Type)()>
static inline bool Set_l043a04(UnkCameraCtrl* self, u64 key, void* in) {
    UnkFieldTable_l043a04* table = Fields_l043a04(self);
    UnkTyped_l043a04 obj = {self->vf_0x0(), self};
    UnkTyped_l043a04 value = {Type(), in};
    if (UnkField_l043a04* field = table->FindField(key))
        return SetReflectedFieldValue(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&obj),
                             reinterpret_cast<UnkReflValueArg*>(&value));
    return false;
}

bool fn_7101043a04(UnkCameraCtrl* controller, const char* name, lua_State* L, s32 mode) {
    switch (name[0]) {
    case 'b': {
        bool value = fn_71010a4900(L, mode) != 0;
        return Set_l043a04<fn_710000d228>(controller, Hash_l043a04(name), &value);
    }
    case 'e': {
        const char* text = luaL_checklstring(L, mode, nullptr);
        u64 key = Hash_l043a04(name);
        UnkField_l043a04* field = Fields_l043a04(controller)->FindField(key);
        if (!field)
            return false;
        const base::reflection::UnkReflObject* type = field->type;
        if (!type || !fn_710009d50c(type->vf_0x0(), fn_710009921c()))
            return false;
        const CEnumType* enumType = reinterpret_cast<const CEnumType*>(type);
        s32 value;
        bool found;
        {
            UnkEnumName_l043a04 id(text, true);
            found = CEnumType_TryGetValueByNameId(enumType, reinterpret_cast<const base::global::CStrId*>(&id), &value);
        }
        if (!found)
            return false;
        UnkTyped_l043a04 obj = {controller->vf_0x0(), controller};
        base::reflection::UnkTypedRef ref;
        fn_71000a16e0(&ref, reinterpret_cast<const char*>(enumType), &value);
        return SetReflectedFieldValue(reinterpret_cast<UnkReflField*>(field), reinterpret_cast<const UnkReflFieldObject*>(&obj),
                             reinterpret_cast<UnkReflValueArg*>(&ref));
    }
    case 'f': {
        f32 value = luaL_checknumber(L, mode);
        return Set_l043a04<fn_710000abbc>(controller, Hash_l043a04(name), &value);
    }
    case 'i': {
        s32 value = luaL_checkinteger(L, mode);
        return Set_l043a04<fn_7100014768>(controller, Hash_l043a04(name), &value);
    }
    case 's': {
        const char* text = luaL_checklstring(L, mode, nullptr);
        u64 key = Hash_l043a04(name);
        CRntString value(text);
        return Set_l043a04<fn_7100007f84>(controller, key, &value);
    }
    case 'v': {
        char* value = fn_71010c1050(reinterpret_cast<char*>(L), mode);
        return Set_l043a04<fn_71000246e4>(controller, Hash_l043a04(name), value);
    }
    }
    return false;
}
