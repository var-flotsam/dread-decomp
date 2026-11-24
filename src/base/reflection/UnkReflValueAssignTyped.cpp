// base::reflection (unit 1635 of tools/tu_map.py): assigns one typed value to another through
// fn_710009d5d4, each passed as a (type, object) reference built from the object's type word at +8; the type
// of the destination selects the conversion. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/CType.h"
#include "types.h"

struct UnkReflTypedObject {
    void* field_0x0;
    const base::reflection::CType* type;
};

bool fn_7100adc81c(UnkReflTypedObject* dst, UnkReflTypedObject* src) {
    base::reflection::UnkTypedRef to = {dst->type, dst};
    base::reflection::UnkTypedRef from = {src->type, src};
    return CType_AreValuesEqual((const char*)dst->type, &to, &from);
}
