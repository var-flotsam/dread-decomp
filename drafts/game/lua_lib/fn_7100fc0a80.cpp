// Unit 2618 (tools/tu_map.py, "lua_lib"): __newindex of the typed values: sets the member named `name` of
// a reflected value from the Lua value on top of the stack. A class sets the field of that name; a
// collection adds the element under the name as a key or, when the name is not a key, sets the element
// at the name read as an index; a pointer passes the assignment on to its pointee. Names and layouts are
// placeholders.
#include <stdlib.h>

#include "../../../lib/lua/lua.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/CPointerType.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU2620.h"
#include "game/UnkTU85.h"
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

struct UnkTypedPair_ovb0016 {
    char* type;
    void* obj;
};

// An object and its type, as the field setter takes it.
struct UnkReflFieldObject {
    char* type;
    void* ptr;
};

// A typed reference to a value (a type and where the value is).
struct UnkReflValueArg {
    const CType* type;
    void* ptr;
};

struct UnkLuaReadScratch_b49a6 {
    u64 words[6];
};

UnkReflValue fn_710009da40(const CType* type, const UnkTypedRef* src, const CType* to);

// A field of a class (CClass::m_array_0xe8's elements, 0xb0 bytes).
struct UnkClassField_lfc0a80 {
    u8 pad_0x0[0x10];
    const CType* type;
    u8 pad_0x18[0x98];
};
static_assert(sizeof(UnkClassField_lfc0a80) == 0xb0, "UnkClassField_lfc0a80 layout");

using UnkAddFn = void (*)(void* obj, void* key, void* element);
using UnkGetFn = UnkReflTypedPtr (*)(void* ptr);

static inline bool IsKindOf(const char* type, const CClass* cls) {
    const CType* t = reinterpret_cast<const UnkReflObject*>(type)->vf_0x0();
    return fn_710009d50c(t, cls);
}

static inline const CCollectionType* AsCollection(char* type) {
    if (!type)
        return nullptr;
    return IsKindOf(type, fn_71000986d0()) ? reinterpret_cast<const CCollectionType*>(type) : nullptr;
}

// The field of a class with the given name hash, or the end of its fields.
static inline UnkClassField_lfc0a80* FindField(const CClass* cls, u64 key) {
    UnkClassField_lfc0a80* fields = static_cast<UnkClassField_lfc0a80*>(cls->m_array_0xe8.data);
    const u64* keys = static_cast<const u64*>(cls->m_array_0xe8.keys);
    for (u32 i = 0; i < cls->m_array_0xe8.count; ++i)
        if (keys[i] == key)
            return &fields[i];
    return &fields[cls->m_array_0xe8.count];
}

// An empty value of `type`.
static inline void InitValue(UnkReflValue* v, const CType* type) {
    v->field_0x0 = nullptr;
    v->type = nullptr;
    v->kind = 2;
    v->field_0x14 = 1;
    fn_71000a2014(v, type);
}

// Where the value lives: inline when it fits in 8 bytes, else through field_0x0.
static inline void* ValuePtr(UnkReflValue* v) {
    if (!v->type)
        return nullptr;
    if (v->field_0x14 == 1 || reinterpret_cast<const CType*>(v->type)->m_size > 8)
        return v->field_0x0;
    return v;
}

// Reads the Lua value on top of the stack into `v`.
static inline void ReadTop(lua_State* L, UnkReflValue* v) {
    UnkTypedRef target = {reinterpret_cast<const CType*>(v->type), ValuePtr(v)};
    UnkLuaReadScratch_b49a6 scratch = {};
    fn_7100fc22a0(L, -1, &target, 2, &scratch);
}

void fn_7100fc0a80(lua_State* L, UnkTypedPair_ovb0016* value, CStrId* name) {
    char* type = value->type;
    if (IsKindOf(type, fn_7100096100())) {
        const CClass* cls = reinterpret_cast<const CClass*>(type);
        UnkClassField_lfc0a80* field = FindField(cls, name->data->Key());
        UnkClassField_lfc0a80* end = &static_cast<UnkClassField_lfc0a80*>(cls->m_array_0xe8.data)[cls->m_array_0xe8.count];
        if (field == end || !field)
            return;
        UnkReflValue v;
        InitValue(&v, field->type);
        ReadTop(L, &v);
        void* obj = value->obj;
        UnkReflFieldObject target = {fn_710009dcd8(type, obj), obj};
        UnkReflValueArg arg;
        fn_71000a1710(&arg, &v);
        fn_71000e7a80(reinterpret_cast<CVariable*>(field), &target, &arg);
        fn_71000a1c60(&v);
        return;
    }

    char* static_type = value->type;
    if (IsKindOf(static_type, fn_71000986d0())) {
        const CCollectionType* coll = reinterpret_cast<const CCollectionType*>(static_type);
        void* obj = value->obj;
        char* actual = fn_710009dcd8(static_type, obj);

        // The name as a key.
        const CType* from = fn_71000081e8();
        const char* text = name->Text();
        UnkTypedRef src = {fn_71000081e8(), &text};
        UnkReflValue key = fn_710009da40(from, &src, coll->m_key);
        if (key.type) {
            UnkReflValue element;
            InitValue(&element, coll->m_element);
            ReadTop(L, &element);
            UnkReflValueArg key_arg;
            fn_71000a1710(&key_arg, &key);
            void* element_ptr = ValuePtr(&element);
            const CCollectionType* c = AsCollection(actual);
            if (c->m_add_copy && reinterpret_cast<const CCollectionType*>(actual) == c && c->m_key == key_arg.type &&
                c->m_element == reinterpret_cast<const CType*>(element.type))
                reinterpret_cast<UnkAddFn>(c->m_add_move)(obj, key_arg.ptr, element_ptr);
            fn_71000a1c60(&element);
        } else {
            // The name as an index.
            char* index_end = nullptr;
            long index = strtol(name->Text(), &index_end, 10);
            if (index_end == name->Text() + name->data->String().length) {
                const CType* index_from = fn_7100010e58();
                UnkTypedRef index_src = {fn_7100010e58(), &index};
                UnkReflValue position = fn_710009da40(index_from, &index_src, coll->m_key);
                if (position.type) {
                    UnkReflValue element;
                    InitValue(&element, coll->m_element);
                    ReadTop(L, &element);
                    UnkReflValueArg position_arg;
                    fn_71000a1710(&position_arg, &position);
                    UnkReflValueArg element_arg;
                    fn_71000a1710(&element_arg, &element);
                    const CCollectionType* c = AsCollection(actual);
                    if (c->m_set_copy && reinterpret_cast<const CCollectionType*>(actual) == c &&
                        c->m_element == element_arg.type)
                        reinterpret_cast<UnkAddFn>(c->m_set_copy)(obj, position_arg.ptr, element_arg.ptr);
                    fn_71000a1c60(&element);
                }
                fn_71000a1c60(&position);
            }
        }
        fn_71000a1c60(&key);
        return;
    }

    char* pointer_type = value->type;
    bool is_pointer = IsKindOf(pointer_type, fn_710009bedc());
    if (!pointer_type || !is_pointer)
        return;
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
    if (pointee.type && pointee.ptr) {
        void* obj = pointee.ptr;
        UnkTypedPair_ovb0016 target = {fn_710009dcd8(pointee.type, obj), obj};
        fn_7100fc0a80(L, &target, name);
    }
}
