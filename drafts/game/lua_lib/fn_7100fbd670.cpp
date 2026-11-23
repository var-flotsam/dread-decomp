// Unit 2618 (tools/tu_map.py, "lua_lib"): __index of the typed values: pushes the member named `name` of a
// reflected value. A class pushes the field of that name (or a closure calling its method of that name);
// a collection pushes one of its methods, or the element under the name as a key or, failing that, at the
// name read as an index; a pointer passes the lookup on to its pointee. Names and layouts are placeholders.
#include <stdlib.h>

#include "../../../lib/lua/lauxlib.h"
#include "../../../lib/lua/lua.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/CPointerType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU85.h"
#include "game/UnkTunableReflValue.h"
#include "game/lua_lib.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CCollectionType;
using base::reflection::CPointerType;
using base::reflection::CType;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypedPtr;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkFormatArgs;
struct UnkTextBuffer200;

struct UnkTypedPair_ovb0016 {
    char* type;
    void* obj;
};

// A typed reference to a value (a type and where the value is).
struct UnkReflValueArg {
    const CType* type;
    void* ptr;
};

UnkReflValue fn_710009da40(const CType* type, const UnkTypedRef* src, const CType* to);
void* fn_71000978e4(char* cls, const CStrId* name);
bool fn_71000e74bc(CVariable* field);
UnkReflTypedPtr fn_71000e755c(CVariable* field, const UnkReflTypedPtr* obj);
int fn_7100fbf2c0(lua_State* L);

// The names of the methods every collection has.
extern CStrId g_unk_71729efef0[7] __attribute__((visibility("hidden")));

// A field of a class (CClass::m_array_0xe8's elements, 0xb0 bytes).
struct UnkClassField_lfbd670 {
    u8 pad_0x0[0xb0];
};

using UnkFindFn = void* (*)(void* obj, void* key);
using UnkAtFn = void* (*)(void* obj, long index);
using UnkGetFn = UnkReflTypedPtr (*)(void* ptr);

// The unit's stack visitor (vtable 0x7101cab628, address point 0x7101cab638), as fn_7100fbde20 uses it.
struct UnkLuaVisitor_lfbd670 {
    void** vtable;
    u32 kind;
    void* state;
    s32 arg0;
    s32 arg1;
    s32 result;
};

// Whether the type descriptor `type` is of the descriptor class `Class` gives.
template <const CClass* (*Class)()>
static inline bool IsKindOf(const char* type) {
    const CType* t = reinterpret_cast<const UnkReflObject*>(type)->vf_0x0();
    return fn_710009d50c(t, Class());
}

template <const CClass* (*Class)()>
static inline char* As(char* type) {
    if (!type)
        return nullptr;
    return IsKindOf<Class>(type) ? type : nullptr;
}

// fn_7100fbde20: pushes the value onto the Lua stack, returning how many values were pushed.
static inline s32 PushValue(lua_State* L, UnkTypedPair_ovb0016* value, s32 arg0, s32 arg1) {
    UnkLuaVisitor_lfbd670 v;
    v.kind = 1;
    v.vtable = g_unk_7101cab638;
    v.state = L;
    v.arg0 = arg0;
    v.arg1 = arg1;
    v.result = 0;
    fn_71000a1784(reinterpret_cast<UnkFormatArgs*>(value), reinterpret_cast<UnkTextBuffer200*>(&v));
    return v.result;
}

// Pushes a closure calling the method `name`.
static inline int PushMethod(lua_State* L, const CStrId* name) {
    lua_pushstring(L, name->Text());
    lua_pushcclosure(L, fn_7100fbf2c0, 1);
    return 1;
}

// The index of the field of a class with the given name hash, or the field count.
static inline u64 FindField(const CClass* cls, u64 key) {
    const u64* keys = static_cast<const u64*>(cls->m_array_0xe8.keys);
    u64 count = cls->m_array_0xe8.count;
    for (u64 i = 0; i < count; ++i)
        if (keys[i] == key)
            return i;
    return count;
}

// Where the value lives: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValuePtr(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || reinterpret_cast<const CType*>(v->type)->m_size > 8)
        return v->field_0x0;
    return v;
}

static inline bool IsCollectionMethod(const CStrId* name) {
    for (const CStrId& method : g_unk_71729efef0)
        if (method == *name)
            return true;
    return false;
}

// The element of the collection at `key` (its m_find), and its type.
static inline void* FindElement(char* actual, void* obj, const UnkReflValueArg* key, const CType** type) {
    const CCollectionType* c = reinterpret_cast<const CCollectionType*>(As<fn_71000986d0>(actual));
    if (!c->m_find || reinterpret_cast<const CCollectionType*>(actual) != c)
        return nullptr;
    if (c->m_key != key->type && !fn_710009d52c(key->type, c->m_key))
        return nullptr;
    void* element = reinterpret_cast<UnkFindFn>(c->m_find)(obj, key->ptr);
    if (element)
        *type = c->m_element;
    return element;
}

// The element of the collection at position `index` (its m_at), and its type.
static inline void* ElementAt(char* actual, void* obj, long index, const CType** type) {
    const CCollectionType* c = reinterpret_cast<const CCollectionType*>(As<fn_71000986d0>(actual));
    if (!c->m_at || reinterpret_cast<const CCollectionType*>(actual) != c)
        return nullptr;
    void* element = reinterpret_cast<UnkAtFn>(c->m_at)(obj, index);
    if (element)
        *type = c->m_element;
    return element;
}

// The element of the collection value named by `name`: under it as a key or, failing that, at it read as
// an index.
static inline int PushElement(lua_State* L, UnkTypedPair_ovb0016* value, CStrId* name, char* static_type) {
    const CCollectionType* coll = reinterpret_cast<const CCollectionType*>(static_type);
    void* obj = value->obj;
    char* actual = fn_710009dcd8(static_type, obj);

    // The name as a key.
    const CType* from = fn_71000081e8();
    const char* text = name->Text();
    UnkTypedRef src = {fn_71000081e8(), &text};
    UnkReflValue key = fn_710009da40(from, &src, coll->m_key);
    const CType* element_type = nullptr;
    void* element = nullptr;
    if (key.type) {
        UnkReflValueArg key_arg;
        fn_71000a1710(&key_arg, &key);
        element = FindElement(actual, obj, &key_arg, &element_type);
    }
    if (!element) {
        // The name as an index.
        char* index_end = nullptr;
        long index = strtol(name->Text(), &index_end, 10);
        if (index_end == name->Text() + name->data->String().length) {
            const CType* index_from = fn_7100010e58();
            UnkTypedRef index_src = {fn_7100010e58(), &index};
            UnkReflValue position = fn_710009da40(index_from, &index_src, coll->m_key);
            if (position.type) {
                UnkReflValueArg position_arg;
                fn_71000a1710(&position_arg, &position);
                element = FindElement(actual, obj, &position_arg, &element_type);
            } else {
                element = ElementAt(actual, obj, index, &element_type);
            }
            fn_71000a1c60(&position);
        }
    }
    int result = 0;
    if (element_type && element) {
        UnkTypedPair_ovb0016 pair = {fn_710009dcd8(const_cast<char*>(reinterpret_cast<const char*>(element_type)), element),
                                     element};
        result = PushValue(L, &pair, 1, 0);
    }
    fn_71000a1c60(&key);
    return result;
}

int fn_7100fbd670(lua_State* L, UnkTypedPair_ovb0016* value, CStrId* name) {
    char* type = value->type;
    if (IsKindOf<fn_7100096100>(type)) {
        const CClass* cls = reinterpret_cast<const CClass*>(type);
        void* obj = value->obj;
        UnkReflTypedPtr target;
        target.type = fn_710009dcd8(type, obj);
        target.ptr = obj;
        u64 index = FindField(cls, name->data->Key());
        UnkClassField_lfbd670* fields = static_cast<UnkClassField_lfbd670*>(cls->m_array_0xe8.data);
        UnkClassField_lfbd670* field = &fields[index];
        UnkClassField_lfbd670* end = &fields[cls->m_array_0xe8.count];
        if (field != end && field) {
            if (fn_71000e74bc(reinterpret_cast<CVariable*>(field))) {
                UnkReflTypedPtr member = fn_71000e755c(reinterpret_cast<CVariable*>(field), &target);
                if (!member.type || !member.ptr)
                    return 0;
                UnkTypedPair_ovb0016 pair = {fn_710009dcd8(member.type, member.ptr), member.ptr};
                return PushValue(L, &pair, 1, 0);
            }
            UnkTunableReflValue v = fn_71000e76dc(reinterpret_cast<UnkTunableFieldHandle*>(field), &target);
            if (!v.type)
                return 0;
            UnkTypedPair_ovb0016 pair = {const_cast<char*>(v.type), ValuePtr(&v)};
            return PushValue(L, &pair, v.field_0x14, 0);
        }

        // Not a field: a method of that name (looked up with a copy of the name), or an error.
        void* method;
        {
            CStrId copy(*name);
            method = fn_71000978e4(As<fn_7100096100>(target.type), &copy);
        }
        if (!method) {
            luaL_error(L, "TypedObject '%s'. Property '%s' not found", cls->m_name.Text(), name->Text());
            lua_pushnil(L);
            return 1;
        }
    } else if (char* static_type = value->type; IsKindOf<fn_71000986d0>(static_type)) {
        if (!IsCollectionMethod(name))
            return PushElement(L, value, name, static_type);
    } else {
        char* pointer_type = value->type;
        bool is_pointer = IsKindOf<fn_710009bedc>(pointer_type);
        if (!pointer_type || !is_pointer)
            return 0;
        const CPointerType* p = reinterpret_cast<const CPointerType*>(pointer_type);
        UnkTypedRef ref;
        fn_71000a174c(&ref, value);
        UnkReflTypedPtr pointee;
        if (p->m_get && ref.type == p)
            pointee = reinterpret_cast<UnkGetFn>(p->m_get)(ref.ptr);
        else {
            pointee.type = nullptr;
            pointee.ptr = nullptr;
        }
        if (!pointee.type || !pointee.ptr)
            return 0;
        void* obj = pointee.ptr;
        UnkTypedPair_ovb0016 target = {fn_710009dcd8(pointee.type, obj), obj};
        return fn_7100fbd670(L, &target, name);
    }
    return PushMethod(L, name);
}
