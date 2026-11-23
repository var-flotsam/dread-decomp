// CChozoCommanderZeroLaserGroundedAttack (tools/tu_map.py): its copy constructor, the implicit copy
// through CChozoCommanderAttack and CChozoCommanderGroundAttack. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CAttack.h"
#include "game/CChozoCommanderAttack.h"

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

CChozoCommanderZeroLaserGroundedAttack::CChozoCommanderZeroLaserGroundedAttack(
    const CChozoCommanderZeroLaserGroundedAttack& other) = default;

extern "C" void fn_7100448b30() __attribute__((alias("_ZN38CChozoCommanderZeroLaserGroundedAttackC2ERKS_")));
