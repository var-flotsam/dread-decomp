// The "DefaultPasteValue" attribute key (CBreakableTileGroupComponent's unit), a copy of fn_7100075240
// (src/base/reflection/UnkReflAttrKeyDefaultValue_b61.cpp): no value type and 2 at +0x10. Names are placeholders.
#include "base/reflection/UnkReflStatics.h"

using base::reflection::CType;

struct UnkRefl_DefaultPasteValue_A5_b64;
template <>
struct base::reflection::UnkReflStatic<UnkRefl_DefaultPasteValue_A5_b64> {
    static constexpr const char* name = "DefaultPasteValue";
    static const CType* type() { return nullptr; }
    static constexpr u32 field_0x10 = 0x2;
};

__attribute__((flatten)) const base::reflection::UnkReflAttrKey& fn_710075a400() {
    return base::reflection::refl_GetAttrKey<UnkRefl_DefaultPasteValue_A5_b64>();
}
