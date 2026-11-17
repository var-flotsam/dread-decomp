// CChozoCommanderZeroLaserGroundedAttack (tools/tu_map.py): its copy assignment. The array and the
// listener's owner are left alone when assigning to itself. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAttack.h"
#include "game_s2/UnkWeakHandle.h"

struct CChozoCommanderAttack : CAttack {
    ~CChozoCommanderAttack() override;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
};

struct CChozoCommanderGroundAttack : CChozoCommanderAttack {
    ~CChozoCommanderGroundAttack() override;
    // Two words: a single 64-bit member would let the store of this class's vtable be removed.
    u32 field_0x48;
    u32 field_0x4c;
};

struct CChozoCommanderZeroLaserGroundedAttack : CChozoCommanderGroundAttack {
    CChozoCommanderZeroLaserGroundedAttack(const CChozoCommanderZeroLaserGroundedAttack& other);
    CChozoCommanderZeroLaserGroundedAttack& operator=(const CChozoCommanderZeroLaserGroundedAttack& other);
    ~CChozoCommanderZeroLaserGroundedAttack() override;
    u64 field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    UnkAttackWords<3> field_0x64;
    u32 field_0x70;
    base::global::CRntVector<u64> field_0x78;
    UnkAttackListener field_0xa0;
};

CChozoCommanderZeroLaserGroundedAttack& CChozoCommanderZeroLaserGroundedAttack::operator=(
    const CChozoCommanderZeroLaserGroundedAttack& other) {
    CChozoCommanderGroundAttack::operator=(other);
    // The plain values up to the words, copied as one block.
    __builtin_memcpy(&field_0x50, &other.field_0x50, 0x14);
    for (int i = 0; i < 3; i++)
        field_0x64.w[i] = other.field_0x64.w[i];
    field_0x70 = other.field_0x70;
    field_0x78 = other.field_0x78;
    __builtin_memcpy(&field_0xa0.field_0x8, &other.field_0xa0.field_0x8, 0x10);
    if (this != &other)
        CopyWeakRef(&field_0xa0.owner.ptr, other.field_0xa0.owner.ptr);
    return *this;
}

extern "C" void fn_7100448cf8() __attribute__((alias("_ZN38CChozoCommanderZeroLaserGroundedAttackaSERKS_")));
