// nn::audio for the port: no audio output until M4 (docs/PORT_PLAN.md). Opening the default
// output fails, which the sound system's set-up (fn_71000f7800) takes as "no sound": it sets the
// sound-off flag and the game runs silent.

#include "nn/audio.h"

namespace nn::audio {

void InitializeAudioOutParameter(AudioOutParameter* parameter) {
    // The console's defaults: 48 kHz, two channels.
    parameter->sample_rate = 48000;
    parameter->channel_count = 2;
    parameter->reserved = 0;
}

Result OpenDefaultAudioOut(AudioOut*, os::SystemEvent*, const AudioOutParameter&) {
    // The event is left as it was, so its owner does not destroy it.
    return ResultNotPorted();
}

void CloseAudioOut(AudioOut*) {
    // Nothing was opened.
}

Result SetAudioDeviceOutputVolume(const AudioDeviceName*, f32) {
    return ResultSuccess();
}

}  // namespace nn::audio
