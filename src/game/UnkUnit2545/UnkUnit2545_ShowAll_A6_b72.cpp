// Unit 2545 (tools/tu_map.py): sets the alpha of both GUI objects of every 0x28-byte entry (count at +0x260,
// objects at +0x198 and +0x1a0 of the first) to 1, the second object first. Names and layouts are placeholders.
#include <stddef.h>

#include "../../game_s7/UnkGuiS7.h"

struct UnkGuiPairEntry_A6_b72 {
    UnkGuiDisplayObject* first;
    UnkGuiDisplayObject* second;
    u8 pad_0x10[0x18];
};

struct UnkUnit2545_A6_b72 {
    u8 pad_0x0[0x198];
    UnkGuiPairEntry_A6_b72 entries[5];
    u32 count;
};
static_assert(offsetof(UnkUnit2545_A6_b72, count) == 0x260, "count");

void fn_7100dd8e34(UnkUnit2545_A6_b72* self) {
    for (u64 i = 0; i < self->count; ++i) {
        UnkGuiPairEntry_A6_b72& e = self->entries[i];
        if (e.second)
            unk_gui::fn_71011cfba8(e.second, 1.0f);
        if (e.first)
            unk_gui::fn_71011cfba8(e.first, 1.0f);
    }
}
