// nn::atk unit: builds the audio renderer parameter from the sound system's settings and returns the
// performance-frame buffer size it needs, rounded up to 64 bytes. Names and layouts are placeholders.
#include <nn/audio.h>

#include "types.h"

struct UnkAtkRendererConfig {
    u32 sample_rate;       // 0x0
    u32 field_0x4;         // 0x4
    u32 voices;            // 0x8
    u8 pad_0xc[4];
    u32 sub_mixes;         // 0x10
    u32 mix_buffers;       // 0x14
    bool field_0x18;
    u8 pad_0x19[2];
    bool field_0x1b;
    bool field_0x1c;
    bool field_0x1d;
    u8 pad_0x1e[3];
    bool field_0x21;
    u8 pad_0x22[5];
    bool field_0x27;
};

u64 fn_71012143bc(void* unused, const UnkAtkRendererConfig* config) {
    nn::audio::AudioRendererParameter param;
    nn::audio::InitializeAudioRendererParameter(&param);
    param.sampleRate = config->sample_rate;
    param.sampleCount = config->sample_rate * 5 / 1000;
    param.mixBufferCount = config->mix_buffers;
    param.subMixCount = config->sub_mixes;
    param.voiceCount = config->voices;
    param.sinkCount = 1;
    param.effectCount = config->field_0x4 + 4;
    param.performanceFrameCount = 0;
    param.isVoiceDropEnabled = config->field_0x21;
    if (config->field_0x18)
        param.performanceFrameCount = 3;
    if (!config->field_0x1b)
        param.effectCount = 0;
    s32 sinks = 2;
    if (config->field_0x1c) {
        param.sinkCount = sinks;
        sinks = 3;
    }
    if (config->field_0x1d)
        param.sinkCount = sinks;
    if (config->field_0x27) {
        param.renderingDevice = nn::audio::AudioRendererRenderingDevice_Cpu;
        param.executionMode = nn::audio::AudioRendererExecutionMode_ManualExecution;
    }
    return (nn::audio::GetRequiredBufferSizeForPerformanceFrames(param) + 63) & ~63ull;
}
