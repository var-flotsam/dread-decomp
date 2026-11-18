// CDoorShieldLifeComponent unit (tools/tu_map.py): whether a hit counts against the shield: the hit's damage flags
// (+0xb4 of the third argument) share a bit with the shield's mask (+0x240) and include one of the bits 0x7000.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDoorShieldLife_l8deae8 {
    u8 pad_0x0[0x240];
    u32 mask;  // 0x240
};

struct UnkDoorShieldHit_l8deae8 {
    u8 pad_0x0[0xb4];
    u32 flags;  // 0xb4
};

u32 fn_71008deae8(UnkDoorShieldLife_l8deae8* self, void* a1, UnkDoorShieldHit_l8deae8* hit, void* a3) {
    u32 flags = hit->flags;
    bool shielded = (self->mask & flags) != 0;
    bool piercing = (flags & 0x7000) != 0;
    return shielded & piercing;
}
