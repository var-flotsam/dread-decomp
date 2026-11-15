// nn::atk unit 2772: builds the audio renderer parameter for a settings block and answers the buffer size a
// circular buffer sink needs under it. Names and layouts are placeholders.
#include <nn/audio.h>

#include "types.h"

struct UnkAtkSystem_l213c30 {
    u8 pad_0x0[0x954];
    u8 flag_0x954;  // 0x954
};

struct UnkAtkSettings_l213c30 {
    u32 sample_rate;  // 0x0
    u32 field_0x4;
    u32 voices;  // 0x8
    u32 field_0xc;
    u32 sub_mixes;    // 0x10
    u32 mix_buffers;  // 0x14
    u8 flag_0x18;
    u8 pad_0x19[2];
    u8 flag_0x1b;
    u8 flag_0x1c;
    u8 flag_0x1d;
    u8 pad_0x1e[3];
    bool flag_0x21;
    u8 pad_0x22[5];
    u8 flag_0x27;
};

size_t fn_7101213c30(UnkAtkSystem_l213c30* self, UnkAtkSettings_l213c30* settings) {
    nn::audio::AudioRendererParameter param;
    nn::audio::InitializeAudioRendererParameter(&param);
    param.sampleRate = settings->sample_rate;
    param.sampleCount = settings->sample_rate * 5 / 1000;
    param.mixBufferCount = settings->mix_buffers;
    param.subMixCount = settings->sub_mixes;
    param.voiceCount = settings->voices;
    param.sinkCount = 1;
    param.effectCount = settings->field_0x4 + 4;
    param.performanceFrameCount = 0;
    param.isVoiceDropEnabled = settings->flag_0x21;
    if (settings->flag_0x18)
        param.performanceFrameCount = 3;
    if (!settings->flag_0x1b)
        param.effectCount = 0;
    s32 next = 2;
    if (settings->flag_0x1c) {
        param.sinkCount = next;
        next = 3;
    }
    if (settings->flag_0x1d)
        param.sinkCount = next;
    if (settings->flag_0x27) {
        param.renderingDevice = nn::audio::AudioRendererRenderingDevice_Cpu;
        param.executionMode = nn::audio::AudioRendererExecutionMode_ManualExecution;
    }
    return nn::audio::GetRequiredBufferSizeForCircularBufferSink(
        &param, self->flag_0x954 ? 2 : 6, settings->field_0xc, nn::audio::SampleFormat_PcmInt16);
}
