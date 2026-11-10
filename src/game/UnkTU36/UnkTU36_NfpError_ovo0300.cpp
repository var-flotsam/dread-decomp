// The NFP error classifier of unit 36.
#include "../../base/UnkUnit36/UnkUnit36_Nfp.h"
#include "types.h"

static inline u32 Finish(UnkUnit36Machine& machine, u32 kind) {
    machine.field_0x2fc = kind;
    return kind;
}

// Classifies the NFP error the machine holds in +0x8 into the kind kept in +0x2fc (and returned);
// no error keeps the kind. Names and values are placeholders.
u32 fn_710002bff0() {
    UnkUnit36Machine& machine = UnkUnit36GetMachine();
    u64 error = machine.field_0x8;
    if (error == 0)
        return machine.field_0x2fc;
    if ((error & 0x3fe1ff) == 0x8073) {
        return Finish(machine, 1);
    } else if ((error & 0x3fffff) == 0xa073) {
        return Finish(machine, 2);
    } else if ((error & 0x3ff1ff) == 0xb073) {
        return Finish(machine, 3);
    } else if ((error & 0x3ff1ff) == 0xc073) {
        return Finish(machine, 4);
    } else {
        switch (error & 0x3fffff) {
        case 0xf073: return Finish(machine, 5);
        case 0x10073: return Finish(machine, 6);
        case 0x11073: return Finish(machine, 7);
        case 0x12073: return Finish(machine, 8);
        case 0x13073: return Finish(machine, 4);
        case 0x14073: return Finish(machine, 4);
        case 0x15073: return Finish(machine, 9);
        default:
            if ((error & 0x3ff1ff) == 0x16073) {
                return Finish(machine, 0xe);
            } else {
                switch (error & 0x3fffff) {
                case 0x17073: return Finish(machine, 0xa);
                case 0x18073: return Finish(machine, 0xf);
                case 0x19073: return Finish(machine, 0xc);
                case 0x1a073: return Finish(machine, 0xd);
                default: return Finish(machine, 0x10);
                }
            }
            break;
        }
    }
}
