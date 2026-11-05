// CCharClassChozoRobotSoldierAIComponent unit (tools/tu_map.py, the name from reflection): picks 2 or 3
// from bit 15 of the fourth argument's word at +0xb4 (a 32-bit bitfield; a plain byte test compiles to a
// sign test). Names and layouts are placeholders.
#include "types.h"

struct UnkFlagged_b66a2 {
    u8 pad_0x0[0xb4];
    u32 low : 15;
    u32 flag : 1;
    u32 high : 16;
};

s32 fn_710046e74c(void* self, void* a, void* b, const UnkFlagged_b66a2* info) {
    return info->flag ? 2 : 3;
}
