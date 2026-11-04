// The "DefaultValue" attribute key (fn_7100075240, in base::input's tunable unit 51): the shape of
// tools/gen_statics.py's attribute keys, with no value type and 2 at +0x10 (gen_statics.py does not
// emit keys without a type). Names are placeholders.
#include "base/reflection/UnkReflStatics.h"

using base::reflection::CType;

struct UnkRefl_DefaultValue_b61;
template <>
struct base::reflection::UnkReflStatic<UnkRefl_DefaultValue_b61> {
    static constexpr const char* name = "DefaultValue";
    static const CType* type() { return nullptr; }
    static constexpr u32 field_0x10 = 0x2;
};

__attribute__((flatten)) const base::reflection::UnkReflAttrKey& fn_7100075240() {
    return base::reflection::refl_GetAttrKey<UnkRefl_DefaultValue_b61>();
}
