// CAutomperAutomaticIrradiationAttack (name from reflection,
// src/reflection/CAutomperAutomaticIrradiationAttack.cpp): the two copy assignments its descriptor
// points at (0x40 bytes, polymorphic: everything after the vtable is copied, level by level of the
// class chain). Member layouts are placeholders.
#include "types.h"

namespace {
struct UnkPolyBase {
    virtual ~UnkPolyBase();
};
struct UnkLevel0 : UnkPolyBase {
    u32 field_0x8[8];
};
struct CAutomperAutomaticIrradiationAttackValue : UnkLevel0 {
    u32 field_0x28[6];
};

__attribute__((always_inline)) inline CAutomperAutomaticIrradiationAttackValue* Assign(
    CAutomperAutomaticIrradiationAttackValue* dst, const CAutomperAutomaticIrradiationAttackValue* src) {
    if (dst != src)
        *dst = *src;
    return dst;
}
}  // namespace

void* fn_71003be350(void* dst, const void* src) {
    return Assign(static_cast<CAutomperAutomaticIrradiationAttackValue*>(dst),
                  static_cast<const CAutomperAutomaticIrradiationAttackValue*>(src));
}
void* fn_71003be37c(void* dst, const void* src) {
    return Assign(static_cast<CAutomperAutomaticIrradiationAttackValue*>(dst),
                  static_cast<const CAutomperAutomaticIrradiationAttackValue*>(src));
}
