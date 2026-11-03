// CDredhedDiveAttack (name from reflection, src/reflection/CDredhedDiveAttack.cpp): the two copy
// assignments its descriptor points at (0x60 bytes, polymorphic: everything after the vtable is
// copied, level by level of the class chain). Member layouts are placeholders.
#include "types.h"

namespace {
struct UnkPolyBase {
    virtual ~UnkPolyBase();
};
struct UnkLevel0 : UnkPolyBase {
    u32 field_0x8[8];
};
struct CDredhedDiveAttackValue : UnkLevel0 {
    u32 field_0x28[6];
    u8 field_0x40[0x19];
};

__attribute__((always_inline)) inline CDredhedDiveAttackValue* Assign(CDredhedDiveAttackValue* dst,
                                                                      const CDredhedDiveAttackValue* src) {
    if (dst != src)
        *dst = *src;
    return dst;
}
}  // namespace

void* fn_71004e6324(void* dst, const void* src) {
    return Assign(static_cast<CDredhedDiveAttackValue*>(dst), static_cast<const CDredhedDiveAttackValue*>(src));
}
void* fn_71004e6368(void* dst, const void* src) {
    return Assign(static_cast<CDredhedDiveAttackValue*>(dst), static_cast<const CDredhedDiveAttackValue*>(src));
}
