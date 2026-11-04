// From unit 372 of tools/tu_map.py (see UnkNavMeshItem.cpp): sets or clears bit 1 of the flags at +0x120.
// Names and layouts are placeholders.
#include "types.h"

struct UnkNavMeshItemFlags_b66a2 {
    u8 pad_0x0[0x120];
    u32 flags;
};

void fn_71002b3ca4(UnkNavMeshItemFlags_b66a2* self, bool on) {
    self->flags = !on ? (self->flags & ~2u) : (self->flags | 2);
}
