// CAbilityShinespark's unit (tools/tu_map.py unit 433): the shinespark's flags for the energy wave. While the
// sub-state (+0x6b0) is in mode 4 and the queried object's state (fn_7100c9e960) is 1 or 9, flags 1 and 2 are
// set; otherwise none. Names and layouts are placeholders.
#include "game/CAbilityLayout.h"
#include "game/CBreakableTileGroup.h"
#include "types.h"

// A set of flags (a u32) that starts empty; the empty set is a shared constant.
struct UnkEnergyWaveFlags_ovb22 {
    u32 bits;
    UnkEnergyWaveFlags_ovb22() : bits(0) {}
    UnkEnergyWaveFlags_ovb22(const UnkEnergyWaveFlags_ovb22& o) : bits(o.bits) {}
};

inline const UnkEnergyWaveFlags_ovb22& NoFlags() {
    static UnkEnergyWaveFlags_ovb22 s_none;
    return s_none;
}

struct UnkShinesparkState_l32b328 {
    u8 pad_0x0[0x10];
    s32 mode;
};

struct UnkShinespark_l32b328 : CAbility {
    u8 pad_0x390[0x6b0 - 0x390];
    UnkShinesparkState_l32b328* state;  // 0x6b0
};

UnkEnergyWaveFlags_ovb22 fn_710032b328(CAbility* entry, void* a, void* b) {
    UnkShinespark_l32b328* self = static_cast<UnkShinespark_l32b328*>(entry);
    if (self->state && self->state->mode == 4) {
        switch (fn_7100c9e960(static_cast<UnkQueryObj*>(a))) {
        case 1:
        case 9: {
            UnkEnergyWaveFlags_ovb22 flags = NoFlags();
            flags.bits |= 3;
            return flags;
        }
        default:
            return NoFlags();
        }
    }
    return NoFlags();
}
