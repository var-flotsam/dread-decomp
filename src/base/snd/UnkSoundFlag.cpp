// From unit 101 of tools/tu_map.py (0x7100104ef4-0x71001061f4; base/snd/UnkSound.h): sets the flag
// at +0x11d of a sound object and, when it changed, tells the helper fn_71000ffc98 through the
// object at +0x78, with the new value and the two floats, which travel on untouched in s0/s1 (the
// helper's value and fade time; tools/forwarded_args.py --calls). The signature is the one
// src/game/UnkAdamDialog.cpp declares.
#include "game/UnkTU101.h"
#include "types.h"

struct UnkSoundSystemATK;
struct UnkSoundVoiceRef_b56;
// Defined in src/base/snd/UnkSoundVoiceSetFloat_b56.cpp.
void fn_71000ffc98(UnkSoundSystemATK* self, UnkSoundVoiceRef_b56* voice, bool flag, f32 value, f32 time);

struct UnkSoundFlagView {
    u8 pad_0x0[0x78];
    void* field_0x78;
    u8 pad_0x80[0x11d - 0x80];
    bool field_0x11d;
};

void fn_7100105ef4(void* sound, f32 time, f32 step, bool arg3) {
    UnkSoundFlagView* self = static_cast<UnkSoundFlagView*>(sound);
    if (self->field_0x11d == arg3)
        return;
    self->field_0x11d = arg3;
    fn_71000ffc98(static_cast<UnkSoundSystemATK*>(self->field_0x78), reinterpret_cast<UnkSoundVoiceRef_b56*>(self),
                  arg3, time, step);
}
