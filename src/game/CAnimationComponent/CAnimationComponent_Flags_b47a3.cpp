// CAnimationComponent unit (tools/tu_map.py): whether any entry of the array at +0x48 (count at +0x50) has a
// given bit set at +0x60. Names and layouts are placeholders.
#include "types.h"

struct UnkAnimEntry_b47a3 {
    u8 unk_0x0[0x60];
    u32 flags;
};

struct UnkAnimFlagsHolder_b47a3 {
    u8 unk_0x0[0x48];
    UnkAnimEntry_b47a3** entries;
    u32 count;
};

bool fn_7101178524(UnkAnimFlagsHolder_b47a3* self, u32 bit) {
    for (UnkAnimEntry_b47a3** it = self->entries; it != self->entries + self->count; ++it) {
        if ((*it)->flags & (1u << bit))
            return true;
    }
    return false;
}
