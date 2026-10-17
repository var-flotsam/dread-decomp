#pragma once

// Reflection attributes: (key, typed value) pairs attached to a type (CType+0x80) or to a
// member field. The keys are guarded static objects named by their own strings
// ("Description", "TrackGroup", ...). Their getters are declared here under fn_<addr>, and
// the comments give each key's string. A value is passed type-erased, as its type
// descriptor plus a pointer.
//
// Only the calls are known. The container, the keys and the insert function keep
// placeholder names.

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

namespace base::reflection {

struct UnkReflAttrKey;  // 0x18 bytes: name (CStrId), u64, u32 (0xf), u16
struct UnkReflAttrs;    // 0x38 bytes, created on first use
struct UnkReflField;    // a member field descriptor; its attributes are at +0x38
// A reflected function (0xb8 bytes, in CClass+0x110). Getter/setter-backed fields register
// one getter and one setter function and then a field that refers to them by index.
struct UnkReflFunction {
    u8 unk_0x0[0x30];
    base::global::CStrId name;
    const CType* return_type;
    void* field_0x40;
    void* invoker;  // the function that calls the accessor
    u8 unk_0x50[0x62];
    u16 field_0xb2;
    u8 unk_0xb4[0x4];
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

// Allocation (size, flags; the flags are 0x3f for the tunable registry), and the registry's
// constructor.
void* fn_71000839a0(u64 size, u32 flags);
void fn_710013ac14(base::reflection::UnkTunableRegistry* self);

// A global CStrId that default values copy (possibly the empty id).
extern base::global::CStrId g_unk_7101d470e0;

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
};

struct UnkReflField {
    u8 unk_0x0[0x38];
    UnkReflAttrs attrs;
};

}  // namespace base::reflection
