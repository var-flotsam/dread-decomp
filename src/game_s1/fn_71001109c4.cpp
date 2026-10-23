// Reflection setter (an invoker) of a pair of f32 at offset 0 of the object: copies the first
// argument's value in and returns an empty value. Like base::reflection::refl_SetFieldValue, for
// an 8-byte struct copied member by member. Called only through its function descriptor.

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
