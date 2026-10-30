#pragma once

// A reflected value returned by a property read: built by fn_71000a18d4 from a type
// descriptor and a pointer to the value (tools/gen_clones.py, kind "field_value"). The layout
// beyond the three fields set here is unknown; names are placeholders.

#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "types.h"

namespace base::reflection {

class CType;

// An argument's value is stored inline when it fits in 8 bytes, else through field_0x0.
struct UnkReflValue {
    void* field_0x0;
    const char* type;  // the value's CType (its size is at +0x18)
    u32 kind;
    u32 field_0x14;  // 1: stored through field_0x0
};

}  // namespace base::reflection

void fn_71000a18d4(base::reflection::UnkReflValue* out, const base::reflection::CType* type, const void* value,
                   u32 by_ref);
// Makes the value a default-constructed value of `type`, destroying the old one first
// (src/base/reflection/fn_71000a18d4.cpp).
void fn_71000a2014(base::reflection::UnkReflValue* value, const base::reflection::CType* type);

namespace base::reflection {

template <class Tag>
struct UnkFieldTraits;  // type(): the value's type descriptor

// Reads the T at `Offset` in the object `obj` points to at +8 and returns it as a reflected
// value (fn_71002bbee8 and its relatives; the first parameter is unused).
template <class Tag, class T, u32 Offset>
UnkReflValue refl_GetFieldValue(void*, char* obj) {
    T value = *reinterpret_cast<T*>(*reinterpret_cast<char**>(obj + 8) + Offset);
    const CType* type = UnkFieldTraits<Tag>::type();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

// Returns the element of the iterator value `obj` points to at +8 as a reflected value, by
// reference (fn_71000cd908 and its copies; the first parameter is unused).
template <class Tag, class T>
UnkReflValue refl_GetIterElementValue(void*, char* obj) {
    auto* v = *reinterpret_cast<UnkIterValue<T>**>(obj + 8);
    T* element = &v->Element();
    const CType* type = UnkFieldTraits<Tag>::type();
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, type, element, true);
    return r;
}

// The same for a polymorphic element, described by its dynamic class (fn_71004b21f8 and its
// copies).
template <class Tag, class T>
UnkReflValue refl_GetIterElementDynValue(void*, char* obj) {
    auto* v = *reinterpret_cast<UnkIterValue<T>**>(obj + 8);
    T* element = &v->Element();
    const CType* type = reinterpret_cast<UnkReflObject*>(element)->vf_0x0();
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    fn_71000a18d4(&r, type, element, true);
    return r;
}

// Copy-assigns the string id at `Offset` in the object `obj` points to at +8 from the first
// argument, and returns an empty value (fn_71002ee040 and its relatives).
template <class Tag, u32 Offset>
UnkReflValue refl_SetFieldStrId(void*, char* obj, UnkReflValue* const* args) {
    UnkReflValue* arg = args[0];
    char* target = *reinterpret_cast<char**>(obj + 8);
    const global::CStrId* value = arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8
                                      ? static_cast<const global::CStrId*>(arg->field_0x0)
                                      : reinterpret_cast<const global::CStrId*>(arg);
    *reinterpret_cast<global::CStrId*>(target + Offset) = *value;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// The same for a trivially copyable T (fn_71002bbf34 and its copies).
template <class Tag, class T, u32 Offset>
UnkReflValue refl_SetFieldValue(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* arg = args[0];
    const T* value = arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8
                         ? static_cast<const T*>(arg->field_0x0)
                         : reinterpret_cast<const T*>(arg);
    *reinterpret_cast<T*>(target + Offset) = *value;
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// The value of a setter's argument: inline when it fits in 8 bytes, else through field_0x0.
template <class T>
inline const T* refl_ArgValue(UnkReflValue* arg) {
    return arg->field_0x14 == 1 || *reinterpret_cast<const u64*>(arg->type + 0x18) > 8
               ? static_cast<const T*>(arg->field_0x0)
               : reinterpret_cast<const T*>(arg);
}

// The empty value a setter returns.
inline UnkReflValue refl_EmptyValue() {
    UnkReflValue r;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    r.kind = 2;
    r.field_0x14 = 1;
    return r;
}

// target(): the object notified; apply(): the follow-up call (setters below).
template <class Tag>
struct UnkSetterTraits;

}  // namespace base::reflection

void fn_7100fdd2b4(char* target, const u64* id);

namespace base::reflection {

// Writes the f32 at `Offset`, raised to at least 0.0001, then passes the property's id (a
// 64-bit hash) to the target object if there is one (fn_710101a504 and its copies).
template <class Tag, u32 Offset, u64 Id>
UnkReflValue refl_SetFieldMinNotify(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 v = *refl_ArgValue<f32>(args[0]);
    *reinterpret_cast<f32*>(target + Offset) = v < 0.0001f ? 0.0001f : v;
    if (char* s = UnkSetterTraits<Tag>::target()) {
        u64 id = Id;
        fn_7100fdd2b4(s, &id);
    }
    return refl_EmptyValue();
}

// Writes the T at `Offset`, then calls apply() (fn_7100f5bd64 and its copies).
template <class Tag, class T, u32 Offset>
UnkReflValue refl_SetFieldApply(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    *reinterpret_cast<T*>(target + Offset) = *refl_ArgValue<T>(args[0]);
    UnkSetterTraits<Tag>::apply();
    return refl_EmptyValue();
}

}  // namespace base::reflection
