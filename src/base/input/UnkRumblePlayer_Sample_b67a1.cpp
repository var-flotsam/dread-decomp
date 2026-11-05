// From unit 48 of tools/tu_map.py: the vibration player's sampling override (fn_7100070fa0).
// Under the player's mutex at +0x1b0 it adds the playback rate (+0x190) to an accumulator (+0x1ac)
// and runs the SDK's sampling step once per whole sample accumulated. Layout placeholders.
#include "base/UnkDefaultAllocator.h"
#include "base/input/UnkHidVibration.h"
#include "types.h"

struct UnkRumbleSampler_b67a1 : nn::hid::VibrationPlayer {
    u8 pad_0xf0[0xa0];
    f32 rate;        // 0x190
    u8 pad_0x194[0x18];
    f32 accumulated; // 0x1ac
    nn::os::MutexType mutex; // 0x1b0
};

void fn_7100070fa0(UnkRumbleSampler_b67a1* self, nn::hid::VibrationValue* value,
                   nn::hid::VibrationNodeConnectionList* connections) {
    fn_710006691c(&self->mutex);
    self->accumulated += self->rate;
    while (self->accumulated >= 1.0f) {
        self->OnNextSampleRequired(value, connections);
        self->accumulated += -1.0f;
    }
    fn_7100066920(&self->mutex);
}
