// tools/tu_map.py unit 254 (no class hint): stores the mode at +0x8c; modes 0 and 1 also hand the
// part held at +0x78 the parameters 0, 6 and 7 (mode 0) or 1 (mode 1) through fn_710019c258
// (src/game/UnkUnit223/UnkUnit223_SetParams.cpp). Like fn_71001c39c8 (unit 255). Names are
// placeholders.
#include "game/UnkTU254.h"
#include "types.h"

struct UnkParams223;
void fn_710019c258(UnkParams223* self, s32 a, s32 b, s32 c);

struct UnkModeOwner254 {
    u8 pad_0x0[0x78];
    UnkParams223* part;  // 0x78
    u8 pad_0x80[0xc];
    s32 mode;  // 0x8c
};

void fn_71001c3770(UnkModeOwner254* self, s32 mode) {
    self->mode = mode;
    if (mode == 0) {
        fn_710019c258(self->part, 0, 6, 7);
    } else if (mode == 1) {
        fn_710019c258(self->part, 0, 6, 1);
    }
}
