// CSamusGunComponent unit (unit 1315 of tools/tu_map.py). Names and layouts are placeholders.
#include "types.h"

struct UnkGunBeamInfo_ovb0070 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkGunBeam_ovb0070 {
    UnkGunBeamInfo_ovb0070* info;
};

struct UnkSamusGunBeams_ovb0070;
bool fn_710084089c(UnkSamusGunBeams_ovb0070* self, const UnkGunBeam_ovb0070* from, const UnkGunBeam_ovb0070* to);

// Whether the beam can change from `from` to `to` without the parent's check (fn_710084089c): both are
// of the first group of four ids, or both of the second group of three.
bool fn_710085bb68(UnkSamusGunBeams_ovb0070* self, const UnkGunBeam_ovb0070* from, const UnkGunBeam_ovb0070* to) {
    switch (from->info->id) {
    case 0xa87817d815624dd3:
    case 0xa7b8a8ac9c26afaa:
    case 0x6803e9b3d5c26ba0:
    case 0xb23e2ea9edd95fb7:
        switch (to->info->id) {
        case 0xa87817d815624dd3:
        case 0xa7b8a8ac9c26afaa:
        case 0x6803e9b3d5c26ba0:
        case 0xb23e2ea9edd95fb7:
            return true;
        }
        break;
    case 0x1125ee6af4f3a61e:
    case 0x3b05b4b082e15ff2:
    case 0x5d523009b8f43e5f:
        switch (to->info->id) {
        case 0x1125ee6af4f3a61e:
        case 0x3b05b4b082e15ff2:
        case 0x5d523009b8f43e5f:
            return true;
        }
        break;
    }
    return fn_710084089c(self, from, to);
}
