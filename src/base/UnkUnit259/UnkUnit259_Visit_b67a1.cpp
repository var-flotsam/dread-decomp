// tools/tu_map.py unit 259 (no class hint): hands every entry of a pointer list whose first mask word
// meets `mask` and whose second meets `mask2` to fn_71001d0d10 (fn_71001d0c84). Names are placeholders.
#include "game/UnkTU259.h"
#include "types.h"

struct UnkMaskedEntry_b67a1 {
    u32 mask;
    u32 mask2;
};

void fn_71001d0c84(void* self, UnkMaskedEntry_b67a1** entries, u32 count, u32 mask, u32 mask2) {
    for (u32 i = 0; i < count; ++i) {
        UnkMaskedEntry_b67a1* e = entries[i];
        if ((e->mask & mask) && (e->mask2 & mask2))
            fn_71001d0d10(self, e);
    }
}
