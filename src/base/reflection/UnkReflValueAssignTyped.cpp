// base::reflection (unit 1635 of tools/tu_map.py): assigns one typed value to another through
// fn_710009d5d4, each passed as a (type, object) reference built from the object's type word at +8; the type
// of the destination selects the conversion. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "types.h"

bool fn_710009d5d4(const char* type, base::reflection::UnkTypedRef* dst, const base::reflection::UnkTypedRef* src);

struct UnkReflTypedObject {
    void* field_0x0;
    const base::reflection::CType* type;
};

bool fn_7100adc81c(UnkReflTypedObject* dst, UnkReflTypedObject* src) {
    base::reflection::UnkTypedRef to = {dst->type, dst};
    base::reflection::UnkTypedRef from = {src->type, src};
    return fn_710009d5d4((const char*)dst->type, &to, &from);
}
