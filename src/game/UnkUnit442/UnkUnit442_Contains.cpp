// tools/tu_map.py unit 442 (beside src/game/CAIComponent.cpp): whether the inline list of 16-bit ids
// (+0x9c, count +0x98) holds `id`. Names are placeholders.
#include "types.h"

struct UnkIdList442 {
    u8 pad_0x0[0x98];
    u32 count;    // 0x98
    u16 ids[1];   // 0x9c
};

bool fn_710035e5c0(const UnkIdList442* self, u16 id) {
    for (u64 i = 0; i < self->count; ++i) {
        if (self->ids[i] == id) {
            return true;
        }
    }
    return false;
}
