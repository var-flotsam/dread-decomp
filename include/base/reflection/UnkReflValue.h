#pragma once

// A reflected value returned by a property read: built by fn_71000a18d4 from a type
// descriptor and a pointer to the value (tools/gen_clones.py, kind "field_value"). The layout
// beyond the three fields set here is unknown; names are placeholders.

#include "base/UnkCollectionIter.h"
#include "base/global/CStrId.h"
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
                   bool flag);

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

}  // namespace base::reflection
