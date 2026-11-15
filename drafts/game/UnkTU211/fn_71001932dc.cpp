// UnkTU211: true when any child in the first list, or else any in the second list, is flagged.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFlagChildA {
    u8 pad_0x0[0x94];
    u8 flag_0x94;
};

struct UnkFlagChildB {
    u8 pad_0x0[0x390];
    u32 flag_0x390;
};

struct UnkFlagOwner {
    u8 pad_0x0[0x20];
    u32 countB_0x20;
    u8 pad_0x24[8];
    u32 countA_0x2c;
    u8 pad_0x30[8];
    UnkFlagChildB** childrenB_0x38;
    u8 pad_0x40[0x38];
    UnkFlagChildA** childrenA_0x78;
    u8 pad_0x80[0x1d8];
    u8 enabled_0x258;
};

bool fn_71001932dc(UnkFlagOwner* self) {
    bool found = false;
    if (self->enabled_0x258 && self->countA_0x2c != 0) {
        u64 i = 0;
        bool flag;
        do {
            flag = self->childrenA_0x78[i++]->flag_0x94 != 0;
        } while (!flag & (i < self->countA_0x2c));
        found = flag;
    }
    if (!found && self->countB_0x20 != 0) {
        u64 i = 0;
        do {
            found = self->childrenB_0x38[i++]->flag_0x390 != 0;
        } while (!found & (i < self->countB_0x20));
    }
    return found;
}
