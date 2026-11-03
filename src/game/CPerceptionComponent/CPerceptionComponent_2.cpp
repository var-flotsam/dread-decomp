// CPerceptionComponent: fn_71009c84c4 hands each entry of the list at +0x48 (count at +0x58) to
// fn_7100f044dc together with the game manager's part at +0x1b00, the counterpart of fn_71009c846c
// (CPerceptionComponent.cpp). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

void fn_7100f044dc(u8* registry, void* entry);

struct UnkPerception2 {
    u8 pad_0x0[0x48];
    void** entries;
    u8 pad_0x50[0x8];
    u32 count;
};

void fn_71009c84c4(UnkPerception2* self) {
    if (self->count == 0)
        return;
    u8* registry = g_unk_7101d47310->pad_0x1b00;
    for (void** it = self->entries, **end = it + self->count; it != end; ++it)
        fn_7100f044dc(registry, *it);
}
