#pragma once

// A reflected value returned by a property read: built by fn_71000a18d4 from a type
// descriptor and a pointer to the value (tools/gen_clones.py, kind "field_value"). The layout
// beyond the three fields set here is unknown; names are placeholders.

#include "types.h"

namespace base::reflection {

class CType;

struct UnkReflValue {
    void* field_0x0;
    void* field_0x8;
    u32 kind;
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
    r.field_0x8 = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

}  // namespace base::reflection
