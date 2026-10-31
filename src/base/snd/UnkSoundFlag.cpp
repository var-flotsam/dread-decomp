// From unit 101 of tools/tu_map.py (0x7100104ef4-0x71001061f4; base/snd/UnkSound.h): sets the flag
// at +0x11d of a sound object and, when it changed, tells the helper fn_71000ffc98 through the
// object at +0x78, with the new value. The signature is the one src/game/UnkAdamDialog.cpp
// declares: the two floats travel in s0/s1 and this function never reads them.
#include "types.h"

void fn_71000ffc98(void* helper, void* sound, bool flag);

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
    fn_71000ffc98(self->field_0x78, self, arg3);
}
