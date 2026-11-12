// CAlternativeActionPlayerComponent unit (tools/tu_map.py): fn_71007124d8 copies each entry's current pair
// into its previous pair over the 0x10-byte entries at +0x48 (count at +0x58), unless the passed-by-value
// argument's flag (byte 4) is set; returns 0. Names are placeholders.
#include "game/CAlternativeActionPlayerComponent.h"
#include "types.h"

struct UnkPair_b69a4 {
    u32 a;
    u32 b;
};

struct UnkSnapEntry_b69a4 {
    UnkPair_b69a4 current;
    UnkPair_b69a4 previous;
};

struct UnkSnapSelf_b69a4 {
    u8 pad_0x0[0x48];
    UnkSnapEntry_b69a4* entries;
    u32 capacity;
    u32 pad_0x54;
    u32 count;
};

u32 fn_71007124d8(UnkAlternativeActionPlayer* player, UnkSnapArg_b69a4 arg) {
    UnkSnapSelf_b69a4* self = reinterpret_cast<UnkSnapSelf_b69a4*>(player);
    if (!arg.skip) {
        for (UnkSnapEntry_b69a4 *it = self->entries, *end = it + self->count; it != end; ++it) {
            // field by field: a struct copy becomes one 8-byte load and store
            it->previous.a = it->current.a;
            it->previous.b = it->current.b;
        }
    }
    return 0;
}
