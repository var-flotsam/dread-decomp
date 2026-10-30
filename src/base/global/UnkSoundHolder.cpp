// From unit 45 of tools/tu_map.py (0x710006b6a4-0x710006d2a0): the constructor of the 0x48-byte
// singleton g_unk_7101d47340 that the engine start-up, the per-frame update and the in-game menu
// create (src/engine/UnkEngineStart.cpp; src/game/GUI/CIngameMenu/fn_7100b2de48.cpp names its first
// word, a sound group). Everything is cleared and the flag at +0x40 set. The layout is a placeholder.
#include "types.h"

struct UnkSoundHolder;

struct UnkSoundHolderPair {
    u64 a;
    u64 b;
};

struct UnkSoundHolderWords {
    UnkSoundHolderPair field_0x0;
    UnkSoundHolderPair field_0x10;
    UnkSoundHolderPair field_0x20;
    UnkSoundHolderPair field_0x30;
    bool field_0x40;
};

// NON_MATCHING(minor): the stores to +0x0 and +0x10 come out in the other order.
void fn_710006cd9c(UnkSoundHolder* holder) {
    auto* self = reinterpret_cast<UnkSoundHolderWords*>(holder);
    self->field_0x20 = {};
    self->field_0x40 = true;
    self->field_0x10 = {};
    self->field_0x0 = {};
    self->field_0x30 = {};
}
