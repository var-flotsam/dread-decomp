// Reflection setter (an invoker) of a pair of f32 at offset 0 of the object: copies the first
// argument's value in and returns an empty value. Like base::reflection::refl_SetFieldValue, for
// an 8-byte struct copied member by member. Called only through its function descriptor.

#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkF32Pair_71001109c4 {
    f32 x;
    f32 y;
};

UnkReflValue fn_71001109c4(void*, char* obj, UnkReflValue* const* args) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    UnkReflValue* arg = args[0];
    auto* field = reinterpret_cast<UnkF32Pair_71001109c4*>(target);
    const UnkF32Pair_71001109c4* value = base::reflection::refl_ArgValue<UnkF32Pair_71001109c4>(arg);
    field->x = value->x;
    field->y = value->y;
    return base::reflection::refl_EmptyValue();
}

// A getter (an invoker) returning the sum of the floats at 0x24 and 0x20 as a reflected float
// (fn_7100110a98).
UnkReflValue fn_7100110a98(void*, char* obj) {
    char* target = *reinterpret_cast<char**>(obj + 8);
    f32 value = *reinterpret_cast<f32*>(target + 0x24) + *reinterpret_cast<f32*>(target + 0x20);
    const base::reflection::CType* type = fn_710000abbc();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}
