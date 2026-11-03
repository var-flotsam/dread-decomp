// tools/tu_map.py unit 251 (no class hint): stores a size (+0x4) and, for the sizes 0x80, 0x100,
// 0x200 and 0x400, the matching code (1, 2, 3, 5) in the part held at +0x78 (+0x20).
// Names are placeholders.
#include "types.h"

struct UnkSizePart251 {
    u8 pad_0x0[0x20];
    s32 code;  // 0x20
};

struct UnkSizeOwner251 {
    u32 field_0x0;
    u32 size;  // 0x4
    u8 pad_0x8[0x70];
    UnkSizePart251* part;  // 0x78
};

void fn_71001bd9b4(UnkSizeOwner251* self, u32 size) {
    self->size = size;
    s32 code;
    switch (size) {
    case 0x80:
        code = 1;
        break;
    case 0x100:
        code = 2;
        break;
    case 0x200:
        code = 3;
        break;
    case 0x400:
        code = 5;
        break;
    default:
        return;
    }
    self->part->code = code;
}
