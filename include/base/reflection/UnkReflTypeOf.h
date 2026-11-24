#pragma once

// The descriptor getters of the attribute and field value types under their addresses, and the
// T -> getter map over them, for the hand-written sources that still name them so. Each is
// refl_GetType<T> (data/functions/), which base/reflection/ReflectedAttributes.h reaches by name
// (ValueType<T>: PrimitiveType<T>::Get, or PointerType<const char*>::Get for a string literal); a
// source moves to those names when it is next worked on.

#include "base/global/CFilePathStrId.h"
#include "base/global/CName.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVector4D.h"
#include "base/reflection/CClass.h"
#include "types.h"

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

}  // namespace base::reflection
