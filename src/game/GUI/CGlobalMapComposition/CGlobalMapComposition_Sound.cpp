// A function in the GUI::CGlobalMapComposition unit: fn_7100b2b4a0 fades out the sound it still holds at
// +0x720 (fn_7100105a80 with 0.2 and 0) and keeps the handle of the one it starts for the state key at
// 0x71729f0240 (fn_7100fc9d78 returns it). Names and layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "game/GUI/CAmiiboComposition.h"
#include "game/sound/SGUISoundEventsSelector.h"
#include "types.h"

struct UnkStateSink;
struct UnkStateKey;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0240;

// The reference block as this file reads it.
struct UnkSoundRef {
    UnkSound* sound;
    s64 serial;
};

// The handle's sound while its serial is still the block's.
static inline UnkSound* SoundOf(const UnkSoundHandle& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1)
        return handle.ref->sound;
    return nullptr;
}

struct UnkGlobalMapComposition {
    u8 pad_0x0[0x720];
    UnkSoundHandle sound;  // 0x720
};

bool fn_7100b2b4a0(UnkGlobalMapComposition* self) {
    if (UnkSound* sound = SoundOf(self->sound))
        fn_7100105a80(sound, 0.2f, 0.0f);
    self->sound = fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0240), self);
    return false;
}
