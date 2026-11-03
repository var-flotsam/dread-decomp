// Unit 252 of tools/tu_map.py (0x71001bdeac-0x71001c071c, no reflected class): switches the part at
// 0xe8 between two modes: enabling sets mode 2 (unless the part's kind is 1), disabling sets 1 or 0x10
// by its kind; two flag bytes follow the choice. The stores have no type-based alias information
// (the original reloads the part after each). Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkAliasU32_252;

struct UnkPart252 {
    u8 pad_0x0[0x18];
    UnkAliasU32_252 mode;
    u8 pad_0x1c[0x14];
    bool field_0x30;
    u8 pad_0x31[0x78];
    bool field_0xa9;
    u8 pad_0xaa[0x9e];
    u32 kind;  // 0x148
};

struct UnkObject252b {
    u8 pad_0x0[0xe8];
    UnkPart252* part;
};

void fn_71001beee4(UnkObject252b* self, bool enable) {
    UnkPart252* part = self->part;
    u32 kind = part->kind;
    bool a9, b30;
    if (enable) {
        if (kind == 1)
            return;
        *(UnkAliasU32_252*)((char*)part + 0x18) = 2;
        a9 = false;
        b30 = true;
    } else {
        *(UnkAliasU32_252*)((char*)part + 0x18) = kind == 1 ? 1 : 0x10;
        a9 = true;
        b30 = false;
    }
    *((char*)self->part + 0xa9) = a9;
    *((char*)self->part + 0x30) = b30;
}
