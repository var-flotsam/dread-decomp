// A function in the GUI::CGlobalMapComposition unit: starts the sound for the state key at 0x71729f0248
// (fn_7100fc9d78 returns its handle in x0/x1) and keeps the handle at +0x710, as fn_7100b2b4a0 does for
// +0x720 (CGlobalMapComposition_Sound.cpp). Names and layouts are placeholders.
#include "game/GUI/CAmiiboComposition.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0248;

struct UnkMapSoundHandle_b47a3 {
    void* ref;
    s64 serial;
};

struct UnkGlobalMapComposition_b47a3 {
    u8 pad_0x0[0x710];
    UnkMapSoundHandle_b47a3 sound;  // 0x710
};

bool fn_7100b2b45c(UnkGlobalMapComposition_b47a3* self) {
    self->sound = reinterpret_cast<UnkMapSoundHandle_b47a3 (*)(UnkStateSink*, UnkStateKey*, void*)>(fn_7100fc9d78)(
        g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0248), self);
    return false;
}
