// CChozoCommanderPowerPulseAttack (name from reflection, src/reflection/CChozoCommanderPowerPulseAttack.cpp):
// the two copy assignments its descriptor points at (0x78 bytes, polymorphic: everything after the
// vtable is copied). Member layouts are placeholders.
#include "types.h"

namespace {
struct UnkPolyBase {
    virtual ~UnkPolyBase();
};

// Each level of the class chain copies its own members, so the runs are not merged.
struct UnkLevel0 : UnkPolyBase {
    u32 field_0x8[8];
};
struct UnkLevel1 : UnkLevel0 {
    u32 field_0x28[8];
};
struct UnkLevel2 : UnkLevel1 {
    u64 field_0x48;
};
struct UnkLevel3 : UnkLevel2 {
    u64 field_0x50;
};
struct CChozoCommanderPowerPulseAttackValue : UnkLevel3 {
    u16 field_0x58[15];
};

__attribute__((always_inline)) inline CChozoCommanderPowerPulseAttackValue* Assign(
    CChozoCommanderPowerPulseAttackValue* dst, const CChozoCommanderPowerPulseAttackValue* src) {
    if (dst != src)
        *dst = *src;
    return dst;
}
}  // namespace

void* fn_7100439ef8(void* dst, const void* src) {
    return Assign(static_cast<CChozoCommanderPowerPulseAttackValue*>(dst),
                  static_cast<const CChozoCommanderPowerPulseAttackValue*>(src));
}
void* fn_7100439f4c(void* dst, const void* src) {
    return Assign(static_cast<CChozoCommanderPowerPulseAttackValue*>(dst),
                  static_cast<const CChozoCommanderPowerPulseAttackValue*>(src));
}
