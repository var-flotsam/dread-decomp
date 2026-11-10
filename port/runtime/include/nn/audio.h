#pragma once

// nn::audio: the low-level audio renderer that the statically linked nn::atk drives (149
// imports, the largest SDK surface after NVN). The boot path opens the renderer
// (OpenAudioRenderer with the parameters and work buffers sized by the Get*BufferSize calls),
// a final mix and the device sink, then renders a frame per tick; voices, sub-mixes, effects
// (delay, reverb, I3DL2 reverb, aux) and the resampler follow as sounds play. Nothing on the
// title screen needs sound, so this is M4 (docs/PORT_PLAN.md: implement over a PC mixer and keep
// nn::atk lifted, or replace nn::atk). The full import list is in data/functions/ (name_source
// symbol, `nn::audio::*`).

#include "nn/nn_common.h"

namespace nn::os {
struct SystemEvent;
}  // namespace nn::os

namespace nn::audio {

// The audio output check in the sound system's set-up (fn_71000f7800,
// src/base/snd/CSoundSystemATK.cpp, which spells these types with the console's layout): it opens
// the default output and closes it again, and leaves sound off when the open fails. The port has
// no output yet (M4), so the open fails and the game runs silent; nn::atk's own calls are the
// silent stand-ins of tools/port_link.py.
struct AudioOutParameter {
    u32 sample_rate;
    u16 channel_count;
    u16 reserved;
};
struct AudioOut;
struct AudioDeviceName;

void InitializeAudioOutParameter(AudioOutParameter* parameter);
Result OpenDefaultAudioOut(AudioOut* out, os::SystemEvent* event, const AudioOutParameter& parameter);
void CloseAudioOut(AudioOut* out);
Result SetAudioDeviceOutputVolume(const AudioDeviceName* device, f32 volume);

// Voices (nn::atk's voice priority, src/atk_id/nn_atk_VoicePriority_b48a3.cpp, and its wave
// voices, src/game/UnkTU2772/UnkTU2772_loader_ovb0268.cpp). The console's layout: one pointer to
// the renderer's voice state, which the port does not read.
struct VoiceType {
    enum PlayState {
        PlayState_Play,
        PlayState_Stop,
        PlayState_Pause,
    };
    void* info;
};
static_assert(sizeof(VoiceType) == 0x8, "nn::audio::VoiceType");

// A buffer of samples queued on a voice (the console's layout).
struct WaveBuffer {
    const void* buffer;
    u64 size;
    s32 start_sample_offset;
    s32 end_sample_offset;
    bool loop;
    bool is_end_of_stream;
    const void* context;
    u64 context_size;
};
static_assert(sizeof(WaveBuffer) == 0x30, "nn::audio::WaveBuffer");

bool IsVoiceValid(const VoiceType* voice);
void SetVoicePriority(VoiceType* voice, s32 priority);
VoiceType::PlayState GetVoicePlayState(const VoiceType* voice);
void SetVoicePlayState(VoiceType* voice, VoiceType::PlayState state);
bool AppendWaveBuffer(VoiceType* voice, const WaveBuffer* buffer);

}  // namespace nn::audio
