// A function in the GUI::CGlobalMapComposition unit: fn_7100b2b4a0 fades out the sound it still holds at
// +0x720 (fn_7100105a80 with 0.2 and 0) and keeps the handle of the one it starts for the state key at
// 0x71729f0240 (fn_7100fc9d78 returns it in x0/x1). Names and layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "types.h"

struct UnkStateSink;
struct UnkStateKey;
extern UnkStateSink* g_unk_717275c380;
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0240;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);

// A handle on a sound: the reference and the serial it was taken with (as in game/UnkAdamDialog.cpp).
struct UnkMapSoundRef {
    UnkSound* sound;
    s64 serial;
};
struct UnkMapSoundHandle {
    UnkMapSoundRef* ref;
    s64 serial;

    UnkSound* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
};

struct UnkGlobalMapComposition {
    u8 pad_0x0[0x720];
    UnkMapSoundHandle sound;  // 0x720
};

bool fn_7100b2b4a0(UnkGlobalMapComposition* self) {
    if (UnkSound* sound = self->sound.Get())
        fn_7100105a80(sound, 0.2f, 0.0f);
    self->sound = reinterpret_cast<UnkMapSoundHandle (*)(UnkStateSink*, UnkStateKey*, void*)>(fn_7100fc9d78)(
        g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0240), self);
    return false;
}
