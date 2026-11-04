// CChozoCommanderKiStrikeAttack unit (tools/tu_map.py, the name from reflection): moves the phase at
// +0x58 on, to 4 from 2 and to 3 from any other. Names and layouts are placeholders.
#include "types.h"

struct UnkKiStrikeAttack_b66a2 {
    u8 pad_0x0[0x58];
    s32 phase;
};

void fn_7100436b64(UnkKiStrikeAttack_b66a2* self) {
    self->phase = self->phase == 2 ? 4 : 3;
}
