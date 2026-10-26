#pragma once

// Reflection attributes: (key, typed value) pairs attached to a type (CType+0x80) or to a
// member field. The keys are guarded static objects named by their own strings
// ("Description", "TrackGroup", ...). Their getters are declared here under fn_<addr>, and
// the comments give each key's string. A value is passed type-erased, as its type
// descriptor plus a pointer.
//
// Only the calls are known. The container, the keys and the insert function keep
// placeholder names.

#include <initializer_list>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CName.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "types.h"

namespace base::reflection {

// An attribute key: a guarded static built by its getter (refl_GetAttrKey<G>, UnkReflStatics.h)
// from its name and its value type's descriptor. The destructor is out of line.
struct UnkReflAttrKey {
    base::global::CStrId name;
    const CType* type;  // the value type
    u32 field_0x10;     // 0xf in all keys seen
    u16 field_0x14;

    UnkReflAttrKey(const base::global::CStrId& name, const CType* type, u32 unk)
        : name(name), type(type), field_0x10(unk), field_0x14(0) {}
    ~UnkReflAttrKey();
};
static_assert(sizeof(UnkReflAttrKey) == 0x18, "UnkReflAttrKey layout");
struct UnkReflAttrs;    // 0x38 bytes, created on first use
struct UnkReflField;    // a member field descriptor; its attributes are at +0x38
// A reflected function (0xb8 bytes, in CClass+0x110). Getter/setter-backed fields register
// one getter and one setter function and then a field that refers to them by index.
struct UnkReflFunction;
// The reflected function's virtual table, as far as the registration functions use it.
struct UnkReflFunctionVtbl {
    void* unk_0x0[9];
    UnkReflAttrs* (*vf_0x48)(UnkReflFunction* self);  // the function's attributes
    void* unk_0x50;
    UnkReflAttrs* (*vf_0x58)(UnkReflFunction* self, u32 index);  // a parameter's attributes
};
struct UnkReflFunction {
    const UnkReflFunctionVtbl* vtbl;
    base::UnkVector<const CType*> params;  // parameter types (fn_7100a19530 adds one)
    base::global::UnkStrId name;
    const CType* return_type;
    void* field_0x40;
    void* invoker;  // the function that calls the accessor
    u8 unk_0x50[0x62];
    u16 field_0xb2;
    u8 unk_0xb4[0x4];

    UnkReflAttrs* Attrs() { return vtbl->vf_0x48(this); }
    UnkReflAttrs* ParamAttrs(u32 index) { return vtbl->vf_0x58(this, index); }
};
static_assert(sizeof(UnkReflFunction) == 0xb8, "function array stride");

struct UnkTunableRegistry;  // 0x58 bytes, created on first use by the CTunable* field functions
struct UnkTunable;          // the global a CTunable* class registers

struct UnkReflValueRef {
    const CType* type;
    const void* value;
};

}  // namespace base::reflection

// Get (creating on first use) a type's attribute container (CType+0x80).
base::reflection::UnkReflAttrs* fn_710009dd6c(base::reflection::CType* type);
// Insert a (key, value) pair.
void fn_71000934b8(base::reflection::UnkReflAttrs* attrs, const base::reflection::UnkReflAttrKey& key,
                   const base::reflection::UnkReflValueRef& value);

// Register a member field: name, type, offset in the object. The two -1s are unknown.
base::reflection::UnkReflField* fn_7100096524(base::reflection::CClass* cls, base::global::CStrId name,
                                              const base::reflection::CType* type, u64 offset, s64 unk_4,
                                              s64 unk_5);

// Add a function named `name` to a class; null if the class already has one by that name.
base::reflection::UnkReflFunction* fn_7100097550(base::reflection::CClass* cls, const base::global::CStrId& name);

// Resize a function's parameter attribute containers (0x38 bytes each, at +0x88) to `count`.
void fn_710009ae5c(base::reflection::UnkReflFunction* fn, u32 count);

// Allocation (size, flags; the flags are 0x3f for the tunable registry), and the registry's
// constructor.
void fn_710013ac14(base::reflection::UnkTunableRegistry* self);

// The CRntString inside the string the global empty id g_unk_7101d470e0 (base/global/CStrId.h)
// points to (+0x10), whose u32 at +0x44 counts these uses; default values copy it.
inline const base::global::CRntString& refl_GlobalRntString() {
    u8* str = reinterpret_cast<u8*>(g_unk_7101d470e0.data);
    ++*reinterpret_cast<u32*>(str + 0x44);
    return *reinterpret_cast<const base::global::CRntString*>(str + 0x10);
}

namespace base::reflection {

// The same for an id in a string-id table (UnkStrIdTable): the count is incremented through a
// may_alias pointer, so the id is reloaded after it, as in the original.
inline const base::global::CRntString& refl_RntStringOf(const base::global::CStrId& id) {
    typedef u32 __attribute__((may_alias)) a_u32;
    ++*reinterpret_cast<a_u32*>(reinterpret_cast<u8*>(id.data) + 0x44);
    return *reinterpret_cast<const base::global::CRntString*>(reinterpret_cast<u8*>(id.data) + 0x10);
}

// A class's member field by name (CClass+0xe8: fields of 0xb0 bytes, keyed by the name's
// hash), or null.
inline UnkReflField* refl_FindField(CClass* cls, const base::global::CName& name) {
    struct Fields {
        struct Slot {
            u8 bytes[0xb0];
        };
        Slot* data;
        const u64* keys;
        u32 count;

        Slot* Find(u64 key) {
            for (u64 i = 0; i < count; ++i)
                if (keys[i] == key)
                    return &data[i];
            return &data[count];
        }
    };
    Fields& fields = *reinterpret_cast<Fields*>(&cls->m_array_0xe8);
    auto* it = fields.Find(name.hash);
    return it == &fields.data[fields.count] ? nullptr : reinterpret_cast<UnkReflField*>(it);
}

// A list value (base::UnkVector's layout) built from an initializer list, moved into a
// reflected value and destroyed like UnkVector::Free.
template <class T>
struct UnkReflList {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Out of line (fn_7100228ca0 for CRntString).
    UnkReflList(std::initializer_list<T> items, u32 step, base::UnkAllocator* allocator, u32 flags);
    UnkReflList(UnkReflList&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkReflList() {
        if (data) {
            for (u32 i = 0; i < count; ++i)
                data[i].~T();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A default value made of a u32 and a list of strings (e.g. "vSegments:*:vPos"), typed by a
// global descriptor.
struct UnkReflStringList {
    u32 field_0x0;
    UnkReflList<base::global::CRntString> items;

    UnkReflStringList(u32 a, UnkReflList<base::global::CRntString>&& list)
        : field_0x0(a), items(static_cast<UnkReflList<base::global::CRntString>&&>(list)) {}
};

}  // namespace base::reflection

namespace base::reflection {

// A table of string ids built once on first use by a weak function-local static (guard and
// object reached through the GOT; destroyed through __cxa_atexit). Default values copy one of
// its ids. CTOR is its constructor's address and names the table; the size is a placeholder.
template <u64 CTOR>
struct UnkStrIdTable {
    base::global::CStrId ids[0x400];

    UnkStrIdTable();
    ~UnkStrIdTable();
};

template <u64 CTOR>
UnkStrIdTable<CTOR>& refl_GetStrIdTable() {
    static UnkStrIdTable<CTOR> s_table;
    return s_table;
}

// Enum default values kept in weak function-local statics, shared program-wide (one per
// ADDR, the static's address): zero, set by a dynamic initialiser (a non-constexpr call), and
// the complement of another one's low byte.
inline u32 refl_EnumZero() {
    return 0;
}

template <u64 ADDR>
const u32& refl_GetEnumZero() {
    static u32 s_value = refl_EnumZero();
    return s_value;
}

template <u64 ADDR, u64 SRC>
const u32& refl_GetEnumNot8() {
    static u32 s_value = u8(~refl_GetEnumZero<SRC>());
    return s_value;
}

}  // namespace base::reflection

// Attribute keys.
const base::reflection::UnkReflAttrKey& fn_7100005070();  // "Version"
const base::reflection::UnkReflAttrKey& fn_7100007e80();  // "Description"
const base::reflection::UnkReflAttrKey& fn_7100075240();  // "DefaultValue"
const base::reflection::UnkReflAttrKey& fn_7100090d50();  // "EventGroup"
const base::reflection::UnkReflAttrKey& fn_7100090e60();  // "TrackGroup"
const base::reflection::UnkReflAttrKey& fn_7100091080();  // "MinValue"
const base::reflection::UnkReflAttrKey& fn_7100091190();  // "MaxValue"
const base::reflection::UnkReflAttrKey& fn_71000912a0();  // "EmptyTrack"

// Type descriptors of attribute value types (each a guarded static named by the type).
const base::reflection::CType* fn_7100005178();  // "unsigned"
const base::reflection::CType* fn_71000081e8();  // "char const*"
const base::reflection::CType* fn_7100009658();  // "base::global::CStrId"
const base::reflection::CType* fn_7100014768();  // "int"
const base::reflection::CType* fn_710000abbc();  // "float"
const base::reflection::CType* fn_710000d228();  // "bool"
const base::reflection::CType* fn_7100024504();  // "base::math::CVector2D"
const base::reflection::CType* fn_71000246e4();  // "base::math::CVector3D"
const base::reflection::CType* fn_71000248c4();  // "base::math::CVector4D"
const base::reflection::CType* fn_7100007f84();  // "base::global::CRntString"
const base::reflection::CType* fn_710000ad9c();  // "base::global::CFilePathStrId"
const base::reflection::CType* fn_710000fd7c();  // "base::global::CName"

namespace base::reflection {

// T -> its type descriptor getter.
template <class T>
struct UnkReflTypeOf;
template <>
struct UnkReflTypeOf<u32> {
    static const CType* Get() { return fn_7100005178(); }
};
template <>
struct UnkReflTypeOf<const char*> {
    static const CType* Get() { return fn_71000081e8(); }
};
template <>
struct UnkReflTypeOf<global::CStrId> {
    static const CType* Get() { return fn_7100009658(); }
};
template <>
struct UnkReflTypeOf<s32> {
    static const CType* Get() { return fn_7100014768(); }
};
template <>
struct UnkReflTypeOf<f32> {
    static const CType* Get() { return fn_710000abbc(); }
};
template <>
struct UnkReflTypeOf<bool> {
    static const CType* Get() { return fn_710000d228(); }
};
template <>
struct UnkReflTypeOf<math::CVector2D> {
    static const CType* Get() { return fn_7100024504(); }
};
template <>
struct UnkReflTypeOf<math::CVector3D> {
    static const CType* Get() { return fn_71000246e4(); }
};
template <>
struct UnkReflTypeOf<math::CVector4D> {
    static const CType* Get() { return fn_71000248c4(); }
};
template <>
struct UnkReflTypeOf<global::CRntString> {
    static const CType* Get() { return fn_7100007f84(); }
};
template <>
struct UnkReflTypeOf<global::CFilePathStrId> {
    static const CType* Get() { return fn_710000ad9c(); }
};
template <>
struct UnkReflTypeOf<global::CName> {
    static const CType* Get() { return fn_710000fd7c(); }
};

struct UnkReflAttrs {
    u8 unk_0x0[0x38];

    template <class T>
    void Add(const UnkReflAttrKey& key, const T& value) {
        fn_71000934b8(this, key, UnkReflValueRef{UnkReflTypeOf<T>::Get(), &value});
    }
    // With an explicit type descriptor (enum values of a field typed by a global descriptor).
    template <class T>
    void Add(const UnkReflAttrKey& key, const CType* type, const T& value) {
        fn_71000934b8(this, key, UnkReflValueRef{type, &value});
    }
    // The same with the type getter called after the value is stored.
    template <class T, auto Get>
    void AddTyped(const UnkReflAttrKey& key, const T& value) {
        fn_71000934b8(this, key, UnkReflValueRef{Get(), &value});
    }
};

struct UnkReflField {
    u8 unk_0x0[0x38];
    UnkReflAttrs attrs;
    UnkReflAttrs attrs_0x70;  // a second container (e.g. "TypeSelectionEnabled" on vector fields)
};

}  // namespace base::reflection
