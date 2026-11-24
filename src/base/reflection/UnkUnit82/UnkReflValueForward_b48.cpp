// Unit 82 (base::reflection): converts a reflected value to another type: builds a typed reference to
// the value (fn_71000a1710) and hands it, with the value's type and the target type, to fn_710009da40.
// Names and layouts are placeholders.
#include "types.h"

#include "base/UnkCloneHelpers.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflValue.h"

using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

// src/base/reflection/CType/CType_Helpers_ovb0053.cpp.
UnkReflValue fn_710009da40(const CType* type, const UnkTypedRef* src, const CType* to);

UnkReflValue ConvertReflValueToType(const UnkReflValue* self, const CType* to) {
    const CType* type = reinterpret_cast<const CType*>(self->type);
    UnkTypedRef ref;
    fn_71000a1710(reinterpret_cast<UnkReflValueArg*>(&ref), self);
    return fn_710009da40(type, &ref, to);
}
